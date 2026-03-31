#pragma once
#include <gui/View.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include "ViewMario.h"


class MainView : public gui::View
{
protected:
    ViewMario _mario;
    gui::GridLayout _gl;

public:
    MainView()
        : _mario()
        , _gl(1, 1)
    {
        gui::GridComposer gc(_gl);
        gc.appendRow(_mario);
        setLayout(&_gl);

        // Inicijalizuj level
        _mario.init();
    }

    void focusOnCanvas()
    {
        _mario.setFocus(true);
    }
};
