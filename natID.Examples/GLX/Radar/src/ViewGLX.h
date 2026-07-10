#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <mu/DebugConsoleLog.h>


class ViewGLX : public glx::View
{
protected:
    
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
    
    //Helper methods (IRenderer -> My Renderer)
    inline Renderer* getRenderer()
    {
        return static_cast<Renderer*>(_pRenderer);
    }
    
    inline const Renderer* getRenderer() const
    {
        return static_cast<const Renderer*>(_pRenderer);
    }

    // Handle mouse input events to control radar targets
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer) {
            gui::Point clickPos = inputDevice.getFramePoint();
            // Convert from bottom-left origin to top-left origin
            auto pMyRenderer = getRenderer();
            clickPos.y = pMyRenderer->getViewportSize().height - clickPos.y;
            pMyRenderer->setLeftClick(clickPos);
            mu::DebugConsoleLog::debug() << "Left click at: " << clickPos.x << ", " << clickPos.y;
        }
    }

    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer) {
            gui::Point clickPos = inputDevice.getFramePoint();
            // Convert from bottom-left origin to top-left origin
            auto pMyRenderer = getRenderer();
            clickPos.y = pMyRenderer->getViewportSize().height - clickPos.y;
            pMyRenderer->setRightClick(clickPos);
            mu::DebugConsoleLog::debug() << "Right click at: " << clickPos.x << ", " << clickPos.y;
        }
    }
};
