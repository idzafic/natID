#pragma once
#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 

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
#include <glx/Texture.h>
#include <glx/TextureDescriptor.h>
#include <glx/DepthStencilDescriptor.h>
#include <glx/DepthStencilState.h>
#include <td/Point.h>
#include <td/Size.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rnd/Perlin.h>
#include <mu/DebugConsoleLog.h>
#include <cmath>
#include <vector>
#include <glx/VertexDescriptor.h>
#include <random>
#include <sstream>

#define WIDTH 1024
#define HEIGHT 1024
#define H0 0.3f
#define HIGHT_SCALE 200.f

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    glx::DepthStencilState _depthState;

    float _perlinNoise[HEIGHT][WIDTH];
    float _vertices[WIDTH * HEIGHT * 3];
    float _colors[WIDTH * HEIGHT * 3];
    unsigned int _indices[(WIDTH - 1) * (HEIGHT - 1) * 6];

    struct TerrainVertex {
        float position[3];
        float color[3];
    };

    glm::mat4 _mvp;
    gui::Size _viewportSize;

    struct TerrainUniforms {
        float mvp[16];
    };

    static_assert(sizeof(TerrainUniforms) == 64, "TerrainUniforms size mismatch - should be 64 bytes");

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");
        path += "/testTerrain.spv";

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            assert(false);
            throw std::runtime_error("Failed to load shader library from testTerrain.spv");
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
            mu::DebugConsoleLog::error() << "Failed to create terrain render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();

        mu::DebugConsoleLog::info() << "Terrain pipeline created with vertex input";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(offsetof(TerrainVertex, position));
        posAttr.setBufferIndex(0);

        glx::VertexAttribute colorAttr = attributes[1];
        colorAttr.setFormat(glx::VertexFormat::Float3);
        colorAttr.setOffset(offsetof(TerrainVertex, color));
        colorAttr.setBufferIndex(0);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(TerrainVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

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
            mu::DebugConsoleLog::error() << "Failed to create depth stencil state";
            assert(false);
        }

        depthDesc.release();
        mu::DebugConsoleLog::info() << "Depth state created";
    }

    void generateTerrain()
    {
        double frequency = 3;
        std::int32_t octaves = 7;
        //std::uint32_t seed = 7272266;
        std::uint32_t seed = std::random_device{}();

        const rnd::PerlinNoise<float, WIDTH> perlin(seed);

        const float fx = perlin.size() / frequency;
        const float fy = perlin.size() / frequency;

        int v = 0;
        int waterVertices = 0;
        int landVertices = 0;

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                float h = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
                _perlinNoise[x][y] = h;

                _vertices[v * 3 + 0] = x;
                _vertices[v * 3 + 1] = h;
                _vertices[v * 3 + 2] = y;

                if (h < H0)
                {
                    // Below sea level
                    float depth = (H0 - h) / H0;
                    _colors[v * 3 + 0] = 0.0f;
                    _colors[v * 3 + 1] = 0.4f * (1.0f - 2 * depth);
                    _colors[v * 3 + 2] = 0.8f * (1.0f - 2 * depth) + 0.2f * depth;
                    _vertices[v * 3 + 1] = 0;
                }
                else
                {
                    float delta = h - H0;
                    _vertices[v * 3 + 1] -= H0;
                    _vertices[v * 3 + 1] *= HIGHT_SCALE;
                    if (delta < 0.03f)
                    {
                        // Beach sand
                        _colors[v * 3 + 0] = 0.76f;
                        _colors[v * 3 + 1] = 0.70f;
                        _colors[v * 3 + 2] = 0.50f;
                    }
                    else if (delta < 0.3f)
                    {
                        _colors[v * 3 + 0] = 0.13f;
                        _colors[v * 3 + 1] = 0.55f - delta;
                        _colors[v * 3 + 2] = 0.13f;
                    }
                    else if (delta < 0.4f)
                    {
                        _colors[v * 3 + 0] = 0.15f;
                        _colors[v * 3 + 1] = 0.52f - delta;
                        _colors[v * 3 + 2] = 0.13f;
                    }
                    else
                    {
                        // Snow
                        _colors[v * 3 + 0] = 1.0f;
                        _colors[v * 3 + 1] = 1.0f;
                        _colors[v * 3 + 2] = 1.0f;
                    }
                }
                v++;
            }
        }

        // Generate triangle indices
        int i = 0;
        for (int y = 0; y < HEIGHT - 1; y++)
        {
            for (int x = 0; x < WIDTH - 1; x++)
            {
                int a = y * WIDTH + x;
                int b = a + 1;
                int c = a + WIDTH;
                int d = c + 1;

                _indices[i++] = a;
                _indices[i++] = c;
                _indices[i++] = b;
                _indices[i++] = b;
                _indices[i++] = c;
                _indices[i++] = d;
            }
        }

        mu::DebugConsoleLog::info() << "Terrain generated with " << (WIDTH * HEIGHT) << " vertices and " << ((WIDTH - 1) * (HEIGHT - 1) * 2) << " triangles";
        mu::DebugConsoleLog::info() << "Water vertices: " << waterVertices << ", Land vertices: " << landVertices;

        mu::DebugConsoleLog::info() << "Sample vertices:";
        for (int i2 = 0; i2 < 10 && i2 < WIDTH * HEIGHT; i2++)
        {
            std::ostringstream oss;
            oss << "  Vertex " << i2 << ": pos(" 
                << _vertices[i2 * 3] << ", " << _vertices[i2 * 3 + 1] << ", " << _vertices[i2 * 3 + 2]
                << ") color(" << _colors[i2 * 3] << ", " << _colors[i2 * 3 + 1] << ", " << _colors[i2 * 3 + 2] << ")";
            mu::DebugConsoleLog::info() << oss.str().c_str();
        }

        std::ostringstream indicesStr;
        indicesStr << "Sample indices: ";
        for (int i3 = 0; i3 < 12 && i3 < (WIDTH - 1) * (HEIGHT - 1) * 6; i3++)
        {
            indicesStr << _indices[i3] << " ";
        }
        mu::DebugConsoleLog::info() << indicesStr.str().c_str();
    }

    void buildBuffers()
    {
        generateTerrain();

        const size_t vertexCount = static_cast<size_t>(WIDTH) * static_cast<size_t>(HEIGHT);

        std::vector<TerrainVertex> interleavedVertices(vertexCount);
        for (size_t i = 0; i < vertexCount; ++i)
        {
            interleavedVertices[i].position[0] = _vertices[i * 3 + 0];
            interleavedVertices[i].position[1] = _vertices[i * 3 + 1];
            interleavedVertices[i].position[2] = _vertices[i * 3 + 2];
            
            interleavedVertices[i].color[0] = _colors[i * 3 + 0];
            interleavedVertices[i].color[1] = _colors[i * 3 + 1];
            interleavedVertices[i].color[2] = _colors[i * 3 + 2];
        }

        const size_t vertexDataSize = interleavedVertices.size() * sizeof(TerrainVertex);
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), interleavedVertices.data(), vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);

        const size_t indexDataSize = sizeof(_indices);
        _indexBuffer = _device.newBuffer(indexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_indexBuffer.contents(), _indices, indexDataSize);
        glx::Buffer::Range indexRange(0, _indexBuffer.length());
        _indexBuffer.didModifyRange(indexRange);

        const size_t uniformDataSize = sizeof(TerrainUniforms);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);

        mu::DebugConsoleLog::info() << "Terrain buffers created with vertex input";
        
 
        mu::DebugConsoleLog::info() << "  Vertex buffer: " << vertexDataSize << " bytes (" << vertexCount << " interleaved vertices)";
 

         mu::DebugConsoleLog::info() << "  Index buffer: " << indexDataSize << " bytes (" << ((WIDTH - 1) * (HEIGHT - 1) * 6) << " indices)";

        
        mu::DebugConsoleLog::info() << "  Uniform buffer: " << uniformDataSize << " bytes (MVP matrix)";

        if (_vertexBuffer.length() != vertexDataSize ||
            _indexBuffer.length() != indexDataSize)
        {
            mu::DebugConsoleLog::error() << "Buffer size mismatch!";
            
    
            mu::DebugConsoleLog::error() << "  Expected vertex: " << vertexDataSize << ", got: " << _vertexBuffer.length();
      

            mu::DebugConsoleLog::error() << "  Expected index: " << indexDataSize << ", got: " << _indexBuffer.length();
        }
        else
        {
            mu::DebugConsoleLog::info() << "All buffer sizes match expectations";
        }
    }

    void updateCamera()
    {
        glm::mat4 model(1.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(WIDTH / 2.0f, 200.0f, HEIGHT + 100.0f),
            glm::vec3(WIDTH / 2.0f, 0.0f, HEIGHT / 2.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        float aspect = 1.0f;
        if (_viewportSize.width > 0 && _viewportSize.height > 0)
        {
            aspect = static_cast<float>(_viewportSize.width) / static_cast<float>(_viewportSize.height);
        }

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 2000.0f);

        _mvp = proj * view * model;

        TerrainUniforms uniforms = {};
        memcpy(uniforms.mvp, glm::value_ptr(_mvp), sizeof(uniforms.mvp));

        memcpy(_uniformBuffer.contents(), &uniforms, sizeof(TerrainUniforms));
        glx::Buffer::Range range(0, sizeof(TerrainUniforms));
        _uniformBuffer.didModifyRange(range);
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        //_viewportSize = gui::Size(800, 600);
        pView->getSize(_viewportSize);

        buildShaders();
        buildDepthState();
        buildBuffers();
        updateCamera();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _uniformBuffer.release();
        _depthState.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        updateCamera();
        
        mu::DebugConsoleLog::info() << "Terrain viewport resized to: " << sz.width << "x" << sz.height;
    }

    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setDepthStencilState(_depthState);

        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 3);

        const int numIndices = (WIDTH - 1) * (HEIGHT - 1) * 6;
        cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, numIndices, glx::IndexType::UInt32, _indexBuffer, 0);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
