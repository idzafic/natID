
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "MainView.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    //ToolBar _toolBar;
    MainView _viewCubeMap;
    bool control = false;
    bool q = false;
public:
    MainWindow()
        : gui::Window(gui::Geometry(0, 0, 1024, 768))
    {
       _mainMenuBar.setAsMain(this);

        setCentralView(&_viewCubeMap);
    }
    
protected:


  ////{
     //   auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
       // if (menuID == 20)
        //{
          //  if (actionID == 10)
            //{
        //        return true;
            //}
        //}
      //  return false;
    //}
    
    //bool onChangedValue(gui::Slider* pSlider) override
    //{
    //    if (pSlider == _toolBar.getSpeedSlider())
    //    {
    //        double val = pSlider->getValue();
    //        _viewCubeMap.updateSpeed((float)val);
    //        return true;
    //    }
    //    return false;
    //}
    //
    //bool onClick(gui::CheckBox* pBtn) override
    //{
    //    if (pBtn == _toolBar.getTextureSwitch())
    //    {
    //        _viewCubeMap.switchTexture();
    //        return true;
    //    }

    //    if (pBtn == _toolBar.getRotateSwitch())
    //    {
    //        _viewCubeMap.switchRotation();
    //        return true;
    //    }
    //    return false;
    //}
    
};
