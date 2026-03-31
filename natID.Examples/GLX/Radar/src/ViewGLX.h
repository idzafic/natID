#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
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

public:
ViewGLX()
    : glx::View({ gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::CursorEnterLeave, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom })
{
    setContinousRenderMode(true);
}
    
Renderer* getRadarRenderer() const { return _pRenderer; }

    // Handle mouse input events to control radar targets
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer) {
            gui::Point clickPos = inputDevice.getFramePoint();
            // Convert from bottom-left origin to top-left origin
            clickPos.y = _pRenderer->getViewportSize().height - clickPos.y;
            _pRenderer->setLeftClick(clickPos);
            mu::DebugConsoleLog::debug() << "Left click at: " << clickPos.x << ", " << clickPos.y;
        }
    }

    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer) {
            gui::Point clickPos = inputDevice.getFramePoint();
            // Convert from bottom-left origin to top-left origin
            clickPos.y = _pRenderer->getViewportSize().height - clickPos.y;
            _pRenderer->setRightClick(clickPos);
            mu::DebugConsoleLog::debug() << "Right click at: " << clickPos.x << ", " << clickPos.y;
        }
    }
};
