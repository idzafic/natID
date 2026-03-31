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
    

    bool onKeyPressed(const gui::Key& key) override
    {
        if (_pRenderer)
        {
            _pRenderer->handleKeyPressed(key);
            return true;
        }
        return false;
    }

    bool onScroll(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto scrollDelta = inputDevice.getScrollDelta();
            bool shiftPressed = inputDevice.getKey().isShiftPressed();
            _pRenderer->handleScroll(scrollDelta, shiftPressed);
            return true;
        }
        return false;
    }

    

    
    bool onZoom(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto scale = inputDevice.getScale();
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleZoom((scale - 1)*0.4 + 1, pt);
            return true;
        }
        return false;
    }

    void onCursorDragged(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            bool shiftPressed = inputDevice.getKey().isShiftPressed();
            _pRenderer->handleMouseDragged(pt, shiftPressed);
        }
    }

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleMousePressed(pt);
        }
    }

public:
    ViewGLX()
        : glx::View({ gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::CursorEnterLeave, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom })
    {
        setContinousRenderMode(true);
        registerForScrollEvents();
    }
    

    void exportFrame()
    {
        td::UINT4 dlgID = 7777;
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
