#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>


class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    
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
            _pRenderer->handleLeftClick(pt);
            reDraw(); // Trigger redraw after input
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (_pRenderer)
        {
            auto pt = inputDevice.getFramePoint();
            _pRenderer->handleRightClick(pt);
            reDraw(); // Trigger redraw after input
        }
    }

public:
    ViewGLX()
    {
        // Enable input events similar to TestGL
        //{gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks}
    }
};
