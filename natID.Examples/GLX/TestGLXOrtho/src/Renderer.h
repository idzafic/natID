#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#define NUM_INSTANCES 1

#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/RenderPipeline.h>
#include <glx/Buffer.h>
#include <glx/Library.h>
#include <glx/Error.h>
#include <glx/Resource.h>
#include <glx/View.h>
#include <glx/Function.h>
#include <glx/VertexDescriptor.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstring>
#include <vector>
#include <deque>
#include <mu/DebugConsoleLog.h>
#include "InstancedObject.h"

struct ObjectVertex {
    float position[2];
};

struct ObjectMesh {
    std::vector<ObjectVertex> vertices;
    std::vector<uint16_t> indices;
};

struct InstanceData {
    glm::vec4 color;
    glm::vec2 translation;
    float scale;
};

struct UniformBufferObject {
    glm::mat4 projection;
};

ObjectMesh createPolygon(int sides, float radius)
{
    ObjectMesh mesh;

    // Center vertex
    mesh.vertices.push_back({{0.0f, 0.0f}});

    float angleStep = 2.0f * glm::pi<float>() / static_cast<float>(sides);
    for (int i = 0; i < sides; ++i)
    {
        float angle = i * angleStep;
        mesh.vertices.push_back({{radius * std::cos(angle), radius * std::sin(angle)}});
    }

    // Triangle fan: center (0) to edge vertices
    for (int i = 0; i < sides; ++i)
    {
        mesh.indices.push_back(0);
        mesh.indices.push_back(static_cast<uint16_t>(i + 1));
        mesh.indices.push_back(static_cast<uint16_t>((i + 1) % sides + 1));
    }

    return mesh;
}

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _uniformBuffer;

    glx::View* _pView = nullptr;

    gui::Size _viewportSize;

    // Pan and zoom
    glm::vec2 _panOffset = glm::vec2(0.0f, 0.0f);
    float _zoom = 1.0f;
    static constexpr float _zoomMin = 0.1f;
    static constexpr float _zoomMax = 50.0f;
    static constexpr float _scrollSpeed = 0.3f;
    static constexpr float _zoomKeyFactor = 1.1f;

    // Drag tracking
    glm::vec2 _lastDragPos = glm::vec2(0.0f, 0.0f);

    using InstancedObject = ::InstancedObject<ObjectVertex, InstanceData>;
    std::deque<InstancedObject> _objects;
    InstancedObject* _crosshairH = nullptr;  // Horizontal line
    InstancedObject* _crosshairV = nullptr;  // Vertical line

protected:

    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load shader library from :shaders: " << err.c_str();
            assert(false);
            throw std::runtime_error("Failed to load shader library from :shaders");
        }

        glx::Function vertexFn = library.newFunction("mainVS");
        glx::Function fragFn = library.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);

        setupVertexDescriptor(desc);

        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        _RP = _device.newRenderPipelineState(desc, error);
        if (!_RP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();

        mu::DebugConsoleLog::info() << "Ortho polygon pipeline created";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Per-vertex: position (vec2)
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float2);
        posAttr.setOffset(offsetof(ObjectVertex, position));
        posAttr.setBufferIndex(0);

        // Per-instance: color (vec4)
        glx::VertexAttribute colorAttr = attributes[1];
        colorAttr.setFormat(glx::VertexFormat::Float4);
        colorAttr.setOffset(offsetof(InstanceData, color));
        colorAttr.setBufferIndex(1);

        
        glx::VertexAttribute transAttr = attributes[2];
        transAttr.setFormat(glx::VertexFormat::Float2);
        transAttr.setOffset(offsetof(InstanceData, translation));
        transAttr.setBufferIndex(1);

       
        glx::VertexAttribute scaleAttr = attributes[3];
        scaleAttr.setFormat(glx::VertexFormat::Float);
        scaleAttr.setOffset(offsetof(InstanceData, scale));
        scaleAttr.setBufferIndex(1);

        // Layout 0: per-vertex data
        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(ObjectVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

       
        glx::VertexBufferLayout layout1 = layouts[1];
        layout1.setStride(sizeof(InstanceData));
        layout1.setStepRate(1);
        layout1.setStepFunction(glx::VertexStepFunction::PerInstance);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }

    void buildObjects()
    {
        // Square (4 sides), Pentagon (5 sides), Circle (64 sides)
        ObjectMesh squareMesh = createPolygon(4, 10.0f);
        ObjectMesh pentagonMesh = createPolygon(5, 10.0f);
        ObjectMesh circleMesh = createPolygon(64, 10.0f);

        _objects.emplace_back(squareMesh.vertices, squareMesh.indices, NUM_INSTANCES);
        _objects.emplace_back(pentagonMesh.vertices, pentagonMesh.indices, NUM_INSTANCES);
        _objects.emplace_back(circleMesh.vertices, circleMesh.indices, NUM_INSTANCES);

        // Place the three objects around center
        // Square: red, left
        _objects[0].addInstance(InstanceData{
            glm::vec4(1.0f, 0.2f, 0.2f, 1.0f),
            glm::vec2(-200.0f, -200.0f),
            10.0f,
        });

        // Pentagon: green, center
        _objects[1].addInstance(InstanceData{
            glm::vec4(0.2f, 1.0f, 0.2f, 1.0f),
            glm::vec2(200.0f, 200.0f),
            10.0f
        });

        // Circle: blue, right
        _objects[2].addInstance(InstanceData{
            glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),
            glm::vec2(-200.0f, 200.0f),
            10.0f
        });

        const size_t uniformDataSize = sizeof(UniformBufferObject);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);

        mu::DebugConsoleLog::info() << "Polygon objects created";
        mu::DebugConsoleLog::info() << "  Square: " << squareMesh.vertices.size() << " vertices";
        mu::DebugConsoleLog::info() << "  Pentagon: " << pentagonMesh.vertices.size() << " vertices";
        mu::DebugConsoleLog::info() << "  Circle: " << circleMesh.vertices.size() << " vertices";
    }

    void buildCrosshair()
    {
        // Horizontal line
        ObjectMesh horizMesh;
        horizMesh.vertices.push_back({{-1.0f, 0.0f}});
        horizMesh.vertices.push_back({{1.0f, 0.0f}});
        horizMesh.indices.push_back(0);
        horizMesh.indices.push_back(1);
        
        _objects.emplace_back(horizMesh.vertices, horizMesh.indices, NUM_INSTANCES);
        _crosshairH = &_objects.back();
        _crosshairH->addInstance(InstanceData{
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            1.0f
        });
        
        // Vertical line
        ObjectMesh vertMesh;
        vertMesh.vertices.push_back({{0.0f, -1.0f}});
        vertMesh.vertices.push_back({{0.0f, 1.0f}});
        vertMesh.indices.push_back(0);
        vertMesh.indices.push_back(1);
        
        _objects.emplace_back(vertMesh.vertices, vertMesh.indices, NUM_INSTANCES);
        _crosshairV = &_objects.back();
        _crosshairV->addInstance(InstanceData{
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            1.0f
        });
        
        mu::DebugConsoleLog::info() << "Crosshair lines created";
    }

    void updateProjection()
    {
        float height = _viewportSize.height / _zoom;
        float width =  _viewportSize.width  / _zoom;   

        float left   =  _panOffset.x;
        float right  =  _panOffset.x + width;
        float bottom =  _panOffset.y + height;
        float top    =  _panOffset.y;

        glm::mat4 proj = glm::ortho(left, right, bottom, top, 0.0f, 1.0f);

        UniformBufferObject ubo;
        ubo.projection = proj;

        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
        _uniformBuffer.didModifyRange(range);
        
        // Update crosshair scale and translation to match viewport
        if (_crosshairH)
        {
            _crosshairH->updateInstance(0, InstanceData{
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec2(_panOffset.x, 0.0f),
                width
            });
        }
        if (_crosshairV)
        {
            _crosshairV->updateInstance(0, InstanceData{
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec2(0.0f, _panOffset.y),
                height
            });
        }
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _pView = pView;
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        InstancedObject::initializeBuffers(_device);

        buildShaders();
        buildObjects();
        buildCrosshair();
        updateProjection();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        mu::DebugConsoleLog::info() << "Ortho polygon renderer initialized";
    }

    ~Renderer()
    {
        InstancedObject::releaseBuffers();

        _uniformBuffer.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
		static bool firstResize = true;
        if(firstResize)
        {
            firstResize = false;
			_panOffset = glm::vec2(-sz.width / 2, -sz.height / 2);
            mu::DebugConsoleLog::info() << "Initial viewport size: " << sz.width << "x" << sz.height;
		}
        _viewportSize = sz;
        updateProjection();

        mu::DebugConsoleLog::debug() << "Viewport resized to: " << sz.width << "x" << sz.height;
    }

    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }

    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isCtrlPressed())
        {
            td::UINT4 ch = key.getCode();
            static int cnt = 0;
            if (cnt++ < 5)
                return;
            mu::DebugConsoleLog::debug() << "Pressed: " << ch;
            gui::Point centerPoint(_viewportSize.width / 2, _viewportSize.height / 2);
            if (ch == '+' || ch == 187)
            {
                handleZoom(_zoomKeyFactor, centerPoint);
            }
            else if (ch == '-' || ch == 189)
            {
                handleZoom(1.0 / _zoomKeyFactor, centerPoint);
            }
        }
    }

    void handleScroll(const gui::Point& scrollDelta, bool shiftPressed)
    {
        float speed = _scrollSpeed / _zoom;
        if (shiftPressed)
            _panOffset.x -= static_cast<float>(scrollDelta.y) * speed;
        else
            _panOffset.y -= static_cast<float>(scrollDelta.y) * speed;
        updateProjection();
    }

    void handleZoom(double scale, const gui::Point& framePoint)
    {
        scale = std::abs(scale);

		double newZoom = glm::clamp(_zoom * static_cast<float>(scale), _zoomMin, _zoomMax);

        double offsetX = (_viewportSize.width / _zoom - _viewportSize.width / (newZoom));
        double offsetY = (_viewportSize.height / _zoom -_viewportSize.height / (newZoom));

		_panOffset.x += offsetX * (framePoint.x/_viewportSize.width);
		_panOffset.y += offsetY * ((_viewportSize.height - framePoint.y) / _viewportSize.height);

		mu::DebugConsoleLog::debug() << "Zoom to point: " << framePoint.x << ", " << _viewportSize.height - framePoint.y << " | New zoom: " << newZoom;

        _zoom = newZoom;

        updateProjection();
    }

    void handleMousePressed(const gui::Point& framePoint)
    {
        mu::DebugConsoleLog::debug() << "Mouse pressed at: " << framePoint.x << ", " << framePoint.y;
        _lastDragPos = glm::vec2(static_cast<float>(framePoint.x), static_cast<float>(framePoint.y));
    }

    void handleMouseDragged(const gui::Point& framePoint, bool shiftPressed)
    {
        glm::vec2 currentPos(static_cast<float>(framePoint.x), static_cast<float>(framePoint.y));

        if (shiftPressed)
        {
            float aspect = _viewportSize.width / _viewportSize.height;
            float halfHeight = 0;// _baseHalfHeight / _zoom;
            float halfWidth = halfHeight * aspect;

            float dx = (currentPos.x - _lastDragPos.x) / _zoom;
            float dy = (currentPos.y - _lastDragPos.y) / _zoom;

            _panOffset.x -= dx;
            _panOffset.y += dy;
            updateProjection();
        }

        _lastDragPos = currentPos;
    }

    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 6);

        // Draw all objects
        for (auto& obj : _objects)
        {
            if (&obj == _crosshairH || &obj == _crosshairV)
            {
                obj.draw(cmdEnc, 0, 1, glx::PrimitiveType::Line);
            }
            else
            {
                obj.draw(cmdEnc, 0, 1);
            }
        }

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
