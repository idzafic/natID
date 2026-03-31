#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#define NUM_INSTANCES 20

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
#include <glx/DepthStencilDescriptor.h>
#include <glx/DepthStencilState.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <deque>
#include <cctype>
#include <mu/DebugConsoleLog.h>
#include "InstancedObject.h"
#include "Extrusion.h"

struct PipeVertex {
    float position[3];
    float normal[3];
};

struct PipeMesh {
    std::vector<PipeVertex> vertices;
    std::vector<uint32_t> indices;
};

inline PipeMesh createPipeMesh(float radius, int segments, const std::vector<glm::vec3>& path)
{
    PipeMesh mesh;
    
    // Define cross-section shape (circle in XY plane)
    std::vector<glm::vec2> shapePoints(segments);
    for (int i = 0; i < segments; ++i)
    {
        float angle = 2.0f * glm::pi<float>() * static_cast<float>(i) / segments;
        shapePoints[i] = glm::vec2(radius * std::cos(angle), radius * std::sin(angle));
    }
    
    // Use Extrusion for positions only - we compute smooth normals analytically
    Extrusion extrusion(shapePoints.data(), segments);
    extrusion.setAddNormals(false);
    extrusion.setAddMidpoints(true, 45.0f);
    extrusion.setKeepCentered(true);
    extrusion.setVisibleSide(true);
    
    std::vector<float> verts;
    std::vector<uint32_t> inds;
    extrusion.Extrude(path, verts, inds);
    
    // Without normals: vertex layout is [x, y, z] = 3 floats per vertex
    const int floatsPerVertex = 3;
    size_t vertexCount = verts.size() / floatsPerVertex;
    mesh.vertices.resize(vertexCount);
    
    // First pass: copy positions
    for (size_t i = 0; i < vertexCount; ++i)
    {
        mesh.vertices[i].position[0] = verts[i * floatsPerVertex + 0];
        mesh.vertices[i].position[1] = verts[i * floatsPerVertex + 1];
        mesh.vertices[i].position[2] = verts[i * floatsPerVertex + 2];
    }
    
    // Second pass: compute smooth normals analytically
    // Vertices are organized in rings of 'segments' vertices each
    // For each ring, normal = normalize(vertex_position - ring_center)
    size_t ringCount = vertexCount / segments;
    for (size_t ring = 0; ring < ringCount; ++ring)
    {
        // Compute ring center (average of all vertices in this ring)
        glm::vec3 center(0.0f);
        for (int seg = 0; seg < segments; ++seg)
        {
            size_t idx = ring * segments + seg;
            center.x += mesh.vertices[idx].position[0];
            center.y += mesh.vertices[idx].position[1];
            center.z += mesh.vertices[idx].position[2];
        }
        center /= static_cast<float>(segments);
        
        // Compute radial smooth normal for each vertex in the ring
        for (int seg = 0; seg < segments; ++seg)
        {
            size_t idx = ring * segments + seg;
            glm::vec3 pos(mesh.vertices[idx].position[0],
                          mesh.vertices[idx].position[1],
                          mesh.vertices[idx].position[2]);
            glm::vec3 normal = glm::normalize(pos - center);
            mesh.vertices[idx].normal[0] = normal.x;
            mesh.vertices[idx].normal[1] = normal.y;
            mesh.vertices[idx].normal[2] = normal.z;
        }
    }
    
    mesh.indices = std::move(inds);
    
    mu::DebugConsoleLog::info() << "Pipe mesh created: " << mesh.vertices.size() << " vertices, " << mesh.indices.size() << " indices";
    
    return mesh;
}

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    glx::View* _pView = nullptr;
    
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _cameraProjectionMatrix;
    
    float _accAngle = 0.0f;
    float _dAngle = 0.01f;
    
    // Camera movement
    glm::vec3 _cameraEye = glm::vec3(3.0f, 3.0f, 10.0f);
    glm::vec3 _cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float _cameraSpeed = 0.1f;
    
    // Camera rotation
    float _cameraYaw = 0.0f;
    float _cameraPitch = 0.0f;
    float _mouseSensitivity = 0.0045f;
    
    // Mouse tracking
    int _lastMouseX = 0;
    int _lastMouseY = 0;
    bool _firstMouseMove = true;
    
    // Keyboard state tracking
    std::unordered_map<char, bool> _keyStates;
    
    // Helper function to get opposite key for conflict resolution
    char getOppositeKey(char key)
    {
        static const std::unordered_map<char, char> opposites = {
            {'W', 'S'},
            {'S', 'W'},
            {'A', 'D'},
            {'D', 'A'}
        };
        auto it = opposites.find(key);
        return (it != opposites.end()) ? it->second : 0;
    }
    
    struct InstanceData {
        float translation[3];
        float color[3];
    };

    using InstancedObject = ::InstancedObject<PipeVertex, InstanceData>;
    std::deque<InstancedObject> _objects;

    struct UniformBufferObject {
        float accAngle;
        float pad0, pad1, pad2;
        glm::vec4 cameraEye;
        glm::mat4 cameraWithProjection;
    };

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
        desc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);

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

        mu::DebugConsoleLog::info() << "Camera and instancing pipeline created";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Vertex buffer (buffer 0) - position + normal
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(offsetof(PipeVertex, position));
        posAttr.setBufferIndex(0);

        glx::VertexAttribute normalAttr = attributes[1];
        normalAttr.setFormat(glx::VertexFormat::Float3);
        normalAttr.setOffset(offsetof(PipeVertex, normal));
        normalAttr.setBufferIndex(0);

        // Instance buffer (buffer 1)
        glx::VertexAttribute translationAttr = attributes[2];
        translationAttr.setFormat(glx::VertexFormat::Float3);
        translationAttr.setOffset(offsetof(InstanceData, translation));
        translationAttr.setBufferIndex(1);

        glx::VertexAttribute colorAttr = attributes[3];
        colorAttr.setFormat(glx::VertexFormat::Float3);
        colorAttr.setOffset(offsetof(InstanceData, color));
        colorAttr.setBufferIndex(1);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(PipeVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        glx::VertexBufferLayout layout1 = layouts[1];
        layout1.setStride(sizeof(InstanceData));
        layout1.setStepRate(1);
        layout1.setStepFunction(glx::VertexStepFunction::PerInstance);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    void buildDepthState()
    {
        glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
        depthDesc.init();
        
        depthDesc.setDepthCompareFunction(glx::CompareFunction::Less);
        depthDesc.setDepthWriteEnabled(true);
        
        _depthState = _device.newDepthStencilState(&depthDesc);
        if (!_depthState.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create depth state";
            assert(false);
        }
        
        mu::DebugConsoleLog::info() << "Depth state created";
        depthDesc.release();
    }
    
    
    void buildObjects()
    {
        // Generate a long random path for the pipe (similar to viewPipes.h example)
        std::vector<glm::vec3> path;
        std::srand(333);
        
        // Create a complex path with 400+ segments in random directions
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                for (int k = 0; k < 20; ++k)
                {
                    // Random direction: up(0), down(1), left(2), right(3), forward(4-5), backward(6)
                    // Forward is 2x more likely than other directions
                    int direction = std::rand() % 7;
                    // Random magnitude: 15-45 units
                    float magnitude = static_cast<float>(std::rand() % 30 + 15) * 0.02f;
                    
                    switch (direction)
                    {
                    case 0: // up (Y+)
                        path.emplace_back(0.0f, magnitude, 0.0f);
                        break;
                    case 1: // down (Y-)
                        path.emplace_back(0.0f, -magnitude, 0.0f);
                        break;
                    case 2: // left (X-)
                        path.emplace_back(-magnitude, 0.0f, 0.0f);
                        break;
                    case 3: // right (X+)
                        path.emplace_back(magnitude, 0.0f, 0.0f);
                        break;
                    case 4: // forward (Z+)
                    case 5: // forward (Z+) - 2x probability
                        path.emplace_back(0.0f, 0.0f, magnitude);
                        break;
                    case 6: // backward (Z-)
                        path.emplace_back(0.0f, 0.0f, -magnitude);
                        break;
                    }
                }
            }
        }
        
        // Create pipe mesh using Extrusion (8 segments for cross-section, 0.1 radius)
        PipeMesh pipeMesh = createPipeMesh(0.1f, 8, path);
        
        _objects.emplace_back(pipeMesh.vertices, pipeMesh.indices, NUM_INSTANCES);
        
        // Generate random instance data for pipes (fixed position, no rotation)
        float colors[][3] = {
            {1.0f, 0.2f, 0.2f},  // Red
            {0.2f, 1.0f, 0.2f},  // Green
            {0.2f, 0.4f, 1.0f},  // Blue
            {1.0f, 1.0f, 0.2f},  // Yellow
            {1.0f, 0.5f, 0.0f},  // Orange
            {0.8f, 0.2f, 1.0f},  // Purple
            {0.0f, 1.0f, 1.0f},  // Cyan
            {1.0f, 0.4f, 0.7f},  // Pink
        };
        
        for (uint32_t i = 0; i < NUM_INSTANCES; i++)
        {
            int colorIdx = i % 8;
            _objects[0].addInstance(InstanceData{
                {
                    (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f,
                },
                {colors[colorIdx][0], colors[colorIdx][1], colors[colorIdx][2]}
            });
        }
        
        const size_t uniformDataSize = sizeof(UniformBufferObject);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::info() << "Pipe objects created";
        mu::DebugConsoleLog::info() << "  Pipe: " << pipeMesh.vertices.size() << " vertices, " << pipeMesh.indices.size() << " indices";
        mu::DebugConsoleLog::info() << "  Path segments: " << path.size();
        mu::DebugConsoleLog::info() << "  Instances: " << NUM_INSTANCES;
    }
    
    void updateCamera()
    {
        const float fov = glm::radians(45.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.5f;
        const float farClip = 200.0f;
        
        _perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        // Use stored camera position and calculate center point ahead of the camera
        glm::vec3 center = _cameraEye + _cameraForward;
        glm::mat4 viewMatrix = glm::lookAt(_cameraEye, center, _cameraUp);
        
        _cameraProjectionMatrix = _perspectiveMatrix * viewMatrix;
        
        UniformBufferObject ubo;
        ubo.accAngle = _accAngle;
        ubo.cameraEye = glm::vec4(_cameraEye, 1.0f);
        ubo.cameraWithProjection = _cameraProjectionMatrix;
        
        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
        _uniformBuffer.didModifyRange(range);
    }
    
    void updateCameraDirection()
    {
        // Update camera forward and right vectors based on yaw and pitch angles
        // Clamp pitch to prevent looking too far up/down
        const float maxPitch = glm::pi<float>() * 0.49f;
        if (_cameraPitch > maxPitch)
            _cameraPitch = maxPitch;
        if (_cameraPitch < -maxPitch)
            _cameraPitch = -maxPitch;
        
        // Calculate new forward vector from yaw and pitch
        float cosYaw = std::cos(_cameraYaw);
        float sinYaw = std::sin(_cameraYaw);
        float cosPitch = std::cos(_cameraPitch);
        float sinPitch = std::sin(_cameraPitch);
        
        _cameraForward.x = sinYaw * cosPitch;
        _cameraForward.y = sinPitch;
        _cameraForward.z = -cosYaw * cosPitch;
        _cameraForward = glm::normalize(_cameraForward);
        
        // Recalculate right vector as perpendicular to forward and world up
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        _cameraRight = glm::normalize(glm::cross(_cameraForward, worldUp));
        
        // Recalculate up vector as perpendicular to forward and right
        _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraForward));
    }
    
    void updateCameraMovement()
    {
        // Update camera position based on held keys
        if (_keyStates['W'])
            _cameraEye += _cameraForward * _cameraSpeed;
        if (_keyStates['S'])
            _cameraEye -= _cameraForward * _cameraSpeed;
        if (_keyStates['A'])
            _cameraEye -= _cameraRight * _cameraSpeed;
        if (_keyStates['D'])
            _cameraEye += _cameraRight * _cameraSpeed;
    }
    

public:
Renderer(glx::View* pView)
    : _device(pView->device())
{
    _pView = pView;
    _commandQueue = _device.newCommandQueue();
    pView->getSize(_viewportSize);

    InstancedObject::initializeBuffers(_device);
    
    // Initialize camera angles - rotated 180 degrees from default
    // Initial forward is (0, 0, 1), which corresponds to yaw=?, pitch=0
    _cameraYaw = glm::pi<float>();
    _cameraPitch = 0.0f;
    updateCameraDirection();
        
    buildShaders();
    buildDepthState();
    buildObjects();
    updateCamera();

    // Configure View to manage depth texture
        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        pView->setClearColor(td::Color(255, 255, 255, 255));
        
        mu::DebugConsoleLog::info() << "Camera and instancing renderer initialized";

}

    ~Renderer()
    {
        InstancedObject::releaseBuffers();
        
        _uniformBuffer.release();
        _depthState.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        // Depth texture is now managed by glx::View - no manual recreation needed
        updateCamera();
        
        mu::DebugConsoleLog::debug() << "Camera and instancing renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        mu::DebugConsoleLog::debug() << "Animation speed updated to: " << _dAngle;
    }
    
    void switchTexture()
    {
        // No-op: textures removed, using solid colors
    }
    
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
    bool prepareNextFrame()
    {
        _accAngle += _dAngle;
        if (_accAngle >= 2.0f * glm::pi<float>())
        {
            _accAngle -= 2.0f * glm::pi<float>();
        }
        
        
        
        updateCameraMovement();
        updateCamera();
        return true;
    }
    
    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = std::toupper(key.getChar());
            _keyStates[ch] = true;
            
            // Clear opposite directional key
            char opposite = getOppositeKey(ch);
            if (opposite)
                _keyStates[opposite] = false;
        }
    }
    
    void handleKeyReleased(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = std::toupper(key.getChar());
            _keyStates[ch] = false;
        }
    }
    
    void handleMouseMoved(const gui::Point& pt)
    {
        if (_firstMouseMove)
        {
            _lastMouseX = pt.x;
            _lastMouseY = pt.y;
            _firstMouseMove = false;
            return;
        }
        
        int deltaX = pt.x - _lastMouseX;
        int deltaY = pt.y - _lastMouseY;
        
        _lastMouseX = pt.x;
        _lastMouseY = pt.y;
        
        // Update yaw and pitch based on mouse movement
        _cameraYaw += deltaX * _mouseSensitivity;
        _cameraPitch += deltaY * _mouseSensitivity;
        
        // Update camera direction vectors
        updateCameraDirection();
    }
    
    void handleLeftClick(const gui::Point& pt)
    {
        mu::DebugConsoleLog::debug() << "Left click at (" << pt.x << ", " << pt.y << ")";
    }
    
    void handleRightClick(const gui::Point& pt)
    {
        mu::DebugConsoleLog::debug() << "Right click at (" << pt.x << ", " << pt.y << ")";
    }

    void draw(glx::View* pView) override
    {
        prepareNextFrame();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setDepthStencilState(_depthState);

        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 6);

        for (auto& obj : _objects)
        {
            obj.draw(cmdEnc, 0, 1);
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
