#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/CommandBuffer.h>
#include <glx/Library.h>
#include <glx/Function.h>
#include <glx/Font.h>
#include <td/Point.h>
#include <td/Size.h>
#include <td/String.h>
#include <gui/InputDevice.h>
#include <vector>
#include <cstring>
#include <stdint.h>
#include <tuple>
#include <mu/DebugConsoleLog.h>


class Renderer : public glx::IRenderer
{
    static constexpr glx::Volume<td::UINT4> _threadsPerGroup = glx::Volume<td::UINT4>(32, 32, 1);
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    
    gui::Size _viewportSize;

    glx::Font _font;

    // Compute shader resources
    glx::Buffer _bufferGameBoard;
    glx::Buffer _bufferViewPort;

    glx::ComputePipelineState _computePipeline;
    glx::RenderPipeline _renderPipeline;

    static constexpr uint32_t GRID_SIZE[2] = { 32*3, 32*2 };

    struct GameBoardData {
        uint32_t width = GRID_SIZE[0];
        uint32_t height = GRID_SIZE[1];
        uint32_t data[GRID_SIZE[0] * GRID_SIZE[1]];

        void setCell(int x, int y, bool value) {
            if (x >= 0 && x < (int)width && y >= 0 && y < (int)height) {
                data[y * width + x] = value;
            }
        }
    } _gameBoard;

    const size_t _bufferElementSize = glx::Buffer::minStorageBufferOffsetAlignment() * (sizeof(GameBoardData) / glx::Buffer::minStorageBufferOffsetAlignment() + 1);

    struct ViewportUBO
    {
        glm::vec4 extentPx;
    } *_pViewportData = nullptr;




    void setUpBuffers()
    {
        _bufferGameBoard = _device.newBuffer(2 * _bufferElementSize, glx::Resource::Options::StorageModePrivate);
        glx::Buffer stagingBuffer = _device.newBuffer(2 * _bufferElementSize, glx::Resource::Options::StorageModeManaged);

        if (!stagingBuffer.isOk() || !_bufferGameBoard.isOk())
        {
            std::exit(10);
        }

        uint32_t *data = (uint32_t *)stagingBuffer.contents();
        std::memset(&_gameBoard, 0, sizeof(GameBoardData));
        
        _gameBoard.width = GRID_SIZE[0];
        _gameBoard.height = GRID_SIZE[1];

        // Blinker pattern (simple oscillator)
        _gameBoard.setCell(0, 1, true);
        _gameBoard.setCell(1, 1, true);
        _gameBoard.setCell(2, 1, true);

        // Beacon oscilator
        _gameBoard.setCell(5, 5, true);
        _gameBoard.setCell(5, 6, true);
        _gameBoard.setCell(6, 5, true);
        _gameBoard.setCell(6, 6, true);

        _gameBoard.setCell(7, 7, true);
        _gameBoard.setCell(7, 8, true);
        _gameBoard.setCell(8, 7, true);
        _gameBoard.setCell(8, 8, true);

        
        //Acorn
        constexpr int x = 40, y = 40;
        _gameBoard.setCell(x, y, true);
        _gameBoard.setCell(x+2, y-1, true);
        _gameBoard.setCell(x+2, y, true);
        _gameBoard.setCell(x+1, y+2, true);
        _gameBoard.setCell(x+2, y+3, true);
        _gameBoard.setCell(x+2, y+4, true);
        _gameBoard.setCell(x+2, y+5, true);

        

        std::memcpy(data, &_gameBoard, sizeof(GameBoardData));
        std::memcpy((uint8_t*)data + _bufferElementSize, &_gameBoard, sizeof(GameBoardData));

        glx::CommandBuffer cmd = _commandQueue.commandBuffer();
        glx::BlitCommandEncoder blit = cmd.blitCommandEncoder();
        blit.copyFromBuffer(stagingBuffer, 0, _bufferGameBoard, 0, 2 * _bufferElementSize);
        
        blit.endEncoding();
        cmd.commit();

        cmd.waitUntilCompleted();

        _bufferViewPort = _device.newBuffer(sizeof(ViewportUBO), glx::Resource::Options::StorageModeManaged);
        if (!_bufferViewPort.isOk())
        {
            std::exit(11);
        }
        
        _pViewportData = (ViewportUBO*)_bufferViewPort.contents();
        std::memset(_pViewportData, 0, sizeof(ViewportUBO));
		stagingBuffer.release();
    }

    
public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        glx::Error e;

        td::String _shadersPath = gui::getResFileName(":shaders");
        glx::Library renderLib = _device.newLibrary(_shadersPath.c_str(), e);

        if (!renderLib.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to load render lib";
            std::exit(1);
        }

        glx::Function vert = renderLib.newFunction("mainVS");
        glx::Function frag = renderLib.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vert);
        desc.setFragmentFunction(frag);
        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm_sRGB);
        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm_sRGB);
        _font.init(_commandQueue, glx::PixelFormat::RGBA8Unorm_sRGB, gui::getResFileName(":font"), 30, 10);

        _renderPipeline = _device.newRenderPipelineState(desc, e);

        glx::Function comp = renderLib.newFunction("mainComp");
        _computePipeline = _device.newComputePipelineState(comp, e);


        if (!_renderPipeline.isOk() || !_computePipeline.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to load render or compute pipeline";
            std::exit(2);
        }
        vert.release();
        frag.release();
        renderLib.release();

        setUpBuffers();
        
    }

	~Renderer()
	{
		_renderPipeline.release();
		_computePipeline.release();
		_bufferGameBoard.release();
		_bufferViewPort.release();
		_commandQueue.release();
		_device.release();
	}

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        if (_pViewportData)
        {
            _pViewportData->extentPx = glm::vec4(sz.width, sz.height, 0.0f, 0.0f);
        }
    }
    
private:
    void draw(glx::View* pView) override
    {
        static uint32_t generationCnt = 0;
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::ComputeCommandEncoder cmdCompute = cmBuff.computeCommandEncoder();
        
        cmdCompute.setComputePipelineState(_computePipeline);
        cmdCompute.setBuffer(_bufferGameBoard, generationCnt % 2 == 0 ? 0 : _bufferElementSize, 3); //read
        cmdCompute.setBuffer(_bufferGameBoard, generationCnt % 2 != 0 ? 0 : _bufferElementSize, 4); //write
        
        static glx::Volume<td::UINT4> threads(GRID_SIZE[0], GRID_SIZE[1], 3);
        cmdCompute.dispatchThreads(threads, _threadsPerGroup);

        cmdCompute.endEncoding();


        glx::RenderCommandEncoder cmdRender = cmBuff.renderCommandEncoder(rpd);
        cmdRender.setRenderPipelineState(_renderPipeline);
        cmdRender.setFragmentBuffer(_bufferGameBoard, generationCnt % 2 != 0 ? 0 : _bufferElementSize, 0);
        cmdRender.setVertexBuffer(_bufferViewPort, 0, 1);
        cmdRender.drawPrimitives(glx::PrimitiveType::TriangleStrip, 0, 4);
        cmdRender.drawFont(_font);

        cmdRender.endEncoding();

        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();


        // Render text with generation counter
        td::String genText;
        genText.format("Gen:%u", generationCnt);
        _font.reset();
        const char* debug = genText.c_str();
        _font.addText(genText, 10, 10, td::Color(0, 255, 0));

        ++generationCnt;
    }

    void release() override
    {
        delete this;
    }
};

