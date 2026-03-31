#pragma once
#include <glx/View.h>
#include "Renderer.h"

class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr; 
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }

public:
    ViewGLX()
    : glx::View({gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::CursorEnterLeave, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorMove, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom })
    {
    }

    // Handle keyboard input for shadow mapping controls (matching OpenGL version)
    bool onKeyPressed(const gui::Key& key) override
    {
        if (!_pRenderer) 
            return false;

        // Arrow keys control light position (matching OpenGL implementation)
        auto virtKey = key.getVirtual();
        switch (virtKey)
        {
        case gui::Key::Virtual::Left:
            _pRenderer->moveLightLeft();
            reDraw();
            return true;
        case gui::Key::Virtual::Right:
            _pRenderer->moveLightRight();
            reDraw();
            return true;
        case gui::Key::Virtual::Up:
            _pRenderer->moveLightUp();
            reDraw();
            return true;
        case gui::Key::Virtual::Down:
            _pRenderer->moveLightDown();
            reDraw();
            return true;
        default:
            break;
        }
        
        // Camera controls (matching OpenGL version)
        char pressedKey = td::toLower(key.getChar());
        switch (pressedKey)
        {
        case ' ':
        {
            bool currentState = _pRenderer->getCameraRotating();
            _pRenderer->setCameraRotating(!currentState);
            reDraw();
            return true;
        }

        case 'a':
        {
            _pRenderer->setCCWRotation(false);
            reDraw();
            return true;
        }
        case 'd':
        {
            _pRenderer->setCCWRotation(true);
            reDraw();
            return true;
        }
        case 'w':
        {
            _pRenderer->setCameraMoveUp(true);
            reDraw();
            return true;
        }

        case 's':
        {
            _pRenderer->setCameraMoveDown(true);
            reDraw();
            return true;
        }
        default:
            break;

        }

        return false;
    }
    
    // Methods for controlling the renderer
    void updateSpeed(float val)
    {
        if (_pRenderer)
        {
            _pRenderer->updateSpeed(val);
            reDraw();
        }
    }
    
    void animate(bool bAnimate)
    {
//        bool bAnimating = isContinousRenderMode();
//        setContinousRenderMode(bAnimate);
        
        if (_pRenderer)
        {
            setContinousRenderMode(bAnimate);
            _pRenderer->switchRotation();
//            reDraw();
        }
    }
    
    // Additional methods to match OpenGL functionality
    void sunColorPicker(const glm::vec3& color)
    {
        if (_pRenderer) {
            _pRenderer->setSunColor(color);
            reDraw();
        }
    }
    
    Renderer* getRenderer()
    {
        return _pRenderer;
    }
};
