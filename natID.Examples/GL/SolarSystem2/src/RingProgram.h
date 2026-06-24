#pragma once
#include <gui/gl/Program.h>

class RingProgram : public gui::gl::Program {
protected:
    void onLinked() override {
        _matrixLocation  = getUniformLocation("mvp");
        _color3Location  = getUniformLocation("color");
        assert(_matrixLocation >= 0 && _color3Location >= 0);
    }
    void onError() override { assert(false); }
};
