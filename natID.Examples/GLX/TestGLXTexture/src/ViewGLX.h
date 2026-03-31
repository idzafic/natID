#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <gui/FileDialog.h>
#include <gui/Texture.h>
#include <gui/Alert.h>
#include <mu/DebugConsoleLog.h>

class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    gui::Texture _frameTexture; //for frame export
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }
    
    // Input handling methods similar to TestGL
    bool onKeyPressed(const gui::Key& key) override
    {
        if (_pRenderer)
        {
            _pRenderer->handleKeyPressed(key);
            reDraw(); // Trigger redraw after input
            return true;
        }
        return false;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleLeftButtonPressed(pt);
            reDraw(); // Trigger redraw after input
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleRightButtonPressed(pt);
            reDraw(); // Trigger redraw after input
        }
    }

public:
    ViewGLX()
        : glx::View({ gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::CursorEnterLeave, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom })
    {
    }
    
    void updateTextScale(float val)
    {
        if (_pRenderer)
        {
            // Update the texture scale in renderer and redraw
            _pRenderer->setTextureScale(val);
            reDraw();
        }
    }
    
    void switchTexture()
    {
        if (_pRenderer)
        {
            _pRenderer->switchTexture();
            reDraw();
        }
    }
    
    void exportFrame()
    {
        td::UINT4 dlgID = 6666;
        auto pFD = gui::SaveFileDialog::show(this, "Export Frame", 
            {
                {"PNG file (no losses)", "*.png"}, 
                {"JPEG file with losses", "*.jpg"},
                {"BMP file (no losses)", "*.bmp"},
                {"TGA file (no losses)", "*.tga"}
            }, 
            dlgID, 
            [this](gui::FileDialog* pFD)
            {
                td::String path = pFD->getFileName();
                if (!path.isNull())
                {
                    mu::DebugConsoleLog::info() << "Exporting frame to: " << path.c_str();
                    
                    // Determine format based on file extension
                    glx::ImageFormat format = glx::ImageFormat::PNG;
                    if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
                    {
                        format = glx::ImageFormat::JPG;
                    }
                    else if (path.endsWith(".bmp"))
                    {
                        format = glx::ImageFormat::BMP;
                    }
                    else if (path.endsWith(".tga"))
                    {
                        format = glx::ImageFormat::TGA;
                    }
                    
                    // Get the current drawable and save it
                    glx::CommandQueue cmdQueue = _pRenderer->getCommandQueue();
                    bool success = saveDrawable(path.c_str(), cmdQueue, format);
                    
                    if (success)
                    {
                        mu::DebugConsoleLog::info() << "Frame exported successfully to: " << path.c_str();
                        gui::Alert::show("Export Success", path);
                    }
                    else
                    {
                        mu::DebugConsoleLog::error() << "Failed to export frame to: " << path.c_str();
                        gui::Alert::show("Export Error", "Failed to save frame");
                    }
                }
            });
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
        {
            if (actionID == 10)
            {
                exportFrame();
                return true;
            }
        }
        return false;
    }
};
