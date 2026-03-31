#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <gui/FileDialog.h>
#include <gui/Alert.h>

#include <mu/DebugConsoleLog.h>

class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }
    

    
    // Input handling methods for 3D animation interaction
    bool onKeyPressed(const gui::Key& key) override
    {
        MU_DEBUG_LOG_INFO("Key pressed");
        if (_pRenderer)
        {
            _pRenderer->handleKeyPressed(key);
//            reDraw(); // // No need (it's continous render mode)
            return true;
        }
        return false;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        MU_DEBUG_LOG_INFO("Primary (left) button press");
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleLeftClick(pt);
            //reDraw(); // No need (it's continous render mode)
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        MU_DEBUG_LOG_INFO("Seondary (right) button press");
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleRightClick(pt);
//            reDraw(); // // No need (it's continous render mode)
        }
    }

public:
    ViewGLX()
        : glx::View({ gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::CursorEnterLeave, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom })
    {
        // Enable input events
        setContinousRenderMode(true);
    }
    

    void updateSpeed(float val)
    {
        if (_pRenderer)
        {
            _pRenderer->updateSpeed(val);
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
        td::UINT4 dlgID = 8888;
        gui::SaveFileDialog::show(this, "Export Frame",
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
                    MU_DEBUG_LOG("Exporting frame to: %s", path.c_str());
                    
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
                        mu::DebugConsoleLog::ok() << "Frame exported successfully to: " << path.c_str();
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
