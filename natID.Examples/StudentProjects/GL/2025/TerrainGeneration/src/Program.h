#pragma once
#include <gui/gl/Program.h>

class Program : public gui::gl::Program
{
protected:
    void onLinked() override
    {
        reserveUniforms(20); // Ostavljamo 20, ima dovoljno mjesta

        // Indeksi ostaju ISTI kao u originalu, da ne bi bilo zabune
        _uniformLocations[0] = getUniformLocation("u_mvp");
        _uniformLocations[1] = getUniformLocation("u_mv");
        _uniformLocations[2] = getUniformLocation("u_n");
        _uniformLocations[3] = getUniformLocation("u_lightDir");
        _uniformLocations[4] = getUniformLocation("u_sunColor");

        // Ovdje NE DIRAMO indekse, samo ćemo prestati koristiti [5]
        _uniformLocations[5] = getUniformLocation("u_texWater"); // I dalje ga dohvaćamo, ali ga nećemo koristiti
        _uniformLocations[6] = getUniformLocation("u_texSand");
        _uniformLocations[7] = getUniformLocation("u_texLightGrass");
        _uniformLocations[8] = getUniformLocation("u_texGrass");
        _uniformLocations[9] = getUniformLocation("u_texDarkGrass");
        _uniformLocations[10] = getUniformLocation("u_texStonyGrass");
        _uniformLocations[11] = getUniformLocation("u_texRock");
        _uniformLocations[12] = getUniformLocation("u_texSnow");

        _uniformLocations[13] = getUniformLocation("fogColor");
        _uniformLocations[14] = getUniformLocation("fogStart");
        _uniformLocations[15] = getUniformLocation("fogEnd");

        // NOVE UNIFORME DODAJEMO NA SLOBODNE INDEKSE
        _uniformLocations[16] = getUniformLocation("u_time");
        _uniformLocations[17] = getUniformLocation("u_cameraPos");

        _matrixLocation = _uniformLocations[0];
    }

    void onError() override
    {
        td::String lastErr = gui::gl::Program::getLastError();
        assert(false);
    }

public:
    Program() {}

    // Sve set* funkcije ostaju ISTE, osim što BRIŠEMO setTextureWater
    void setMVP(const glm::mat4x4& mvp) { setUniformValue(_uniformLocations[0], mvp); }
    void setLight(const glm::vec3& dir, const glm::vec3& color) { setUniformValue(_uniformLocations[3], dir); setUniformValue(_uniformLocations[4], color); }
    void setMV(const glm::mat4& mv) { setUniformValue(_uniformLocations[1], mv); }
    void setN(const glm::mat4& n) { setUniformValue(_uniformLocations[2], n); }

    // Ovdje je bila setTextureWater, sada je obrisana.
    void setTextureSand(int texID) { _pTextures->setUniform(_uniformLocations[6], gui::gl::Textures::Unit::T1, texID); }
    void setTextureLightGrass(int texID) { _pTextures->setUniform(_uniformLocations[7], gui::gl::Textures::Unit::T2, texID); }
    void setTextureGrass(int texID) { _pTextures->setUniform(_uniformLocations[8], gui::gl::Textures::Unit::T3, texID); }
    void setTextureDarkGrass(int texID) { _pTextures->setUniform(_uniformLocations[9], gui::gl::Textures::Unit::T4, texID); }
    void setTextureStonyGrass(int texID) { _pTextures->setUniform(_uniformLocations[10], gui::gl::Textures::Unit::T5, texID); }
    void setTextureRock(int texID) { _pTextures->setUniform(_uniformLocations[11], gui::gl::Textures::Unit::T6, texID); }
    void setTextureSnow(int texID) { _pTextures->setUniform(_uniformLocations[12], gui::gl::Textures::Unit::T7, texID); }

    void setFog(const glm::vec3& color, float start, float end)
    {
        setUniformValue(_uniformLocations[13], color);
        setUniformValue(_uniformLocations[14], start);
        setUniformValue(_uniformLocations[15], end);
    }

    // NOVE FUNKCIJE
    void setTime(float time) { setUniformValue(_uniformLocations[16], time); }
    void setCameraPos(const glm::vec3& pos) { setUniformValue(_uniformLocations[17], pos); }
};