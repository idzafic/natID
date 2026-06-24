#pragma once
#include <gui/gl/Program.h>
#include <gui/gl/Textures.h>

class BodyProgram : public gui::gl::Program {
    td::INT4 _texLoc = -1;
protected:
    void onLinked() override {
        _matrixLocation = getUniformLocation("mvp");
        _texLoc         = getUniformLocation("tex");
        assert(_matrixLocation >= 0 && _texLoc >= 0);
    }
    void onError() override { assert(false); }
};
