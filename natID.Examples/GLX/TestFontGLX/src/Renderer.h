#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/CommandBuffer.h>
#include <glx/Font.h>
#include <td/Point.h>
#include <td/Size.h>
#include <td/String.h>
#include <gui/InputDevice.h>
#include <mu/DebugConsoleLog.h>



class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    
    glx::Font _font1;  // Stack-allocated Font objects
    glx::Font _font2;
    bool _font1Initialized = false;
    bool _font2Initialized = false;
    
    gui::Size _viewportSize;
    td::UINT1 _usedFont = 0;

protected:
    void loadTextures()
    {
        mu::DebugConsoleLog::info() << "Loading font textures";

        
        // Set maximum characters that can be rendered (e.g., 1000 characters)
        const size_t maxChars = 1000;
        
        td::String fontPath1 = gui::getResFileName(":font1");
        _font1Initialized = _font1.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, fontPath1.c_str(), 320.0, maxChars);
        if (!_font1Initialized)
        {
            mu::DebugConsoleLog::error() << "Failed to initialize font1, location is " << fontPath1.c_str();
        }
        else
        {
            mu::DebugConsoleLog::debug() << "  Font1 isOk: " << (_font1.getTexture().isOk() ? "YES" : "NO");
            
            // Add test text to font1
            _font1.addText((const char *)u8"a\u00E4d", 100.0, 100.0, td::Color(255, 153, 0, 255));
        }
        
        td::String fontPath2 = gui::getResFileName(":font2");
        _font2Initialized = _font2.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, fontPath2.c_str(), 32.0, maxChars);
        if (!_font2Initialized)
        {
            mu::DebugConsoleLog::error() << "Failed to initialize font2, location is: " << fontPath2.c_str();
        }
        else
        {
            mu::DebugConsoleLog::debug() << "  Font2 isOk: " << (_font2.getTexture().isOk() ? "YES" : "NO");
            
            // Add test text to font2
            _font2.addText("Test Font 2", 100.0, 200.0, td::Color(255, 0, 0, 255));
        }
        
        if (!_font1Initialized && !_font2Initialized)
        {
            mu::DebugConsoleLog::error() << "CRITICAL ERROR: No fonts loaded! Textures will be transparent.";
        }
    }
    
public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        _viewportSize = gui::Size(800, 600);

        loadTextures();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        
        // Font will automatically retrieve viewport size during draw
        mu::DebugConsoleLog::ok() << "Texture renderer initialized";
        mu::DebugConsoleLog::debug() << "  Viewport: " << _viewportSize.width << "x" << _viewportSize.height;
    }

    ~Renderer()
    {
        
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        
    }
    
    void switchTexture()
    {
        _usedFont = (_usedFont == 0) ? 1 : 0;
        mu::DebugConsoleLog::debug() << "Switched to font " << (int)_usedFont;
    }
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }

private:
    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();
        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);
        cmdEnc.endEncoding();
        cmBuff.commit(); //for clearing the screen

        glx::Font* currentFont = nullptr;
        bool fontInitialized = false;
        
        if (_usedFont == 0)
        {
            currentFont = &_font1;
            fontInitialized = _font1Initialized;
        }
        else
        {
            currentFont = &_font2;
            fontInitialized = _font2Initialized;
        }
        
        if (fontInitialized && currentFont->isOk())
        {            
            currentFont->draw(pView, false);
        }

        
        glx::CommandBuffer cmBuff2 = _commandQueue.commandBuffer();
        cmBuff2.presentDrawable(pView->currentDrawable());
        cmBuff2.commit(); //for presenting

    }

    void release() override
    {
        delete this;
    }
};

