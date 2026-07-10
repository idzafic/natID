#pragma once
#include <glx/View.h>
#include "Renderer.h"

class ViewGLX : public glx::View
{
protected:
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
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
                getRenderer()->moveLightLeft();
            //reDraw();
            return true;
        case gui::Key::Virtual::Right:
                getRenderer()->moveLightRight();
            //reDraw();
            return true;
        case gui::Key::Virtual::Up:
                getRenderer()->moveLightUp();
            //reDraw();
            return true;
        case gui::Key::Virtual::Down:
                getRenderer()->moveLightDown();
            //reDraw();
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
            auto pRenderer = getRenderer();
            bool currentState = pRenderer->getCameraRotating();
            pRenderer->setCameraRotating(!currentState);
            //reDraw();
            return true;
        }

        case 'a':
        {
            getRenderer()->setCCWRotation(false);
            //reDraw();
            return true;
        }
        case 'd':
        {
            getRenderer()->setCCWRotation(true);
            //reDraw();
            return true;
        }
        case 'w':
        {
            getRenderer()->setCameraMoveUp(true);
            //reDraw();
            return true;
        }

        case 's':
        {
            getRenderer()->setCameraMoveDown(true);
            //reDraw();
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
            getRenderer()->updateSpeed(val);
            //reDraw();
        }
    }
    
    void animate(bool bAnimate)
    {
//        bool bAnimating = isContinousRenderMode();
//        setContinousRenderMode(bAnimate);
        
        if (_pRenderer)
        {
            setContinousRenderMode(bAnimate);
            getRenderer()->switchRotation();
//            reDraw();
        }
    }
    
    // Additional methods to match OpenGL functionality
    void sunColorPicker(const glm::vec3& color)
    {
        if (_pRenderer) {
            getRenderer()->setSunColor(color);
//            reDraw();
        }
    }
    
};
