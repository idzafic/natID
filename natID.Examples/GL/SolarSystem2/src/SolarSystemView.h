#pragma once

#include <gui/gl/CameraView.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>
#include <gui/gl/SkyBox.h>
#include <gui/gl/Text.h>
#include <gui/gl/Context.h>
#include <gui/Key.h>
#include <gui/InputDevice.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BodyProgram.h"
#include "RingProgram.h"

#include <chrono>
#include <vector>
#include <cmath>

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

class SolarSystemView : public gui::gl::CameraView
{
    gui::gl::Camera _camera;
    glm::mat4       _vp;
    glm::mat4       _skyVP {1.0f};   

    gui::gl::Buffer _sphereBuffer;
    gui::gl::Buffer _orbitBuffer;

    BodyProgram _bodyProgram;
    RingProgram _ringProgram;

    gui::gl::Textures _textures;

    gui::gl::SkyBox _skyBoxStarfield;
    gui::gl::SkyBox _skyBoxBlue;

    glm::mat4 _mSun, _mMercury, _mVenus, _mEarth, _mMoon;
    glm::mat4 _mMars, _mJupiter, _mSaturn, _mUranus, _mNeptune;

    std::vector<glm::mat4> _ringMtx;

    gui::gl::Text _text;

    glm::vec3 _earthPos  {0, 0, 0};
    glm::vec3 _saturnPos {0, 0, 0};

    bool  _skyBoxExtra  = true;
    int   _planetView   = 0;
    bool  _freeCam      = false;
    bool  _onRotate     = false;
    float _planetSpeed  = 0.1f;
    float _sceneRotateY = 0.0f;
    float _sceneRotateX = 0.0f;

    std::chrono::steady_clock::time_point _startTime = std::chrono::steady_clock::now();
    float _lastFrame = 0.0f;
    float _deltaTime = 0.0f;

    float _lastDragX    = 0.0f;
    float _lastDragY    = 0.0f;
    bool  _firstDragPt  = true;

    glm::vec3 _flyPos { 0.0f, 1200.0f, 2500.0f };
    float _camYaw   = -90.0f;
    float _camPitch = -25.6f;
    bool  _keys[256] = { false };
    float _moveSpeed = 600.0f;
    bool  _firstMouseFrame = true;
    static constexpr float MOUSE_SENS = 0.1f;

    glm::vec3 camFront() const
    {
        float y = glm::radians(_camYaw);
        float p = glm::radians(_camPitch);
        return glm::normalize(glm::vec3(std::cos(y) * std::cos(p),
                                        std::sin(p),
                                        std::sin(y) * std::cos(p)));
    }

    enum TexIdx : td::BYTE {
        TEX_SUN = 0, TEX_MERCURY, TEX_VENUS, TEX_EARTH, TEX_MOON,
        TEX_MARS, TEX_JUPITER, TEX_SATURN, TEX_URANUS, TEX_NEPTUNE,
        TEX_CM_STARFIELD,
        TEX_CM_BLUE,
        TEX_COUNT
    };

    static constexpr int ORBIT_PTS = 256;

    float getTime() const
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = now - _startTime;
        return dt.count();
    }

    glm::mat4 sceneMat() const
    {
        glm::mat4 m(1.0f);
        m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
        m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
        return m;
    }

    void buildHUD()
    {
        struct PlanetInfo { const char* name; const char* speed; const char* gravity; };
        static const PlanetInfo planets[8] = {
            { "MERCURY", "47.87 km/s", "0.38 g" },
            { "VENUS",   "35.02 km/s", "0.90 g" },
            { "EARTH",   "29.76 km/s", "1.00 g" },
            { "MARS",    "24.13 km/s", "0.38 g" },
            { "JUPITER", "13.07 km/s", "2.55 g" },
            { "SATURN",  " 9.67 km/s", "1.12 g" },
            { "URANUS",  " 6.84 km/s", "0.97 g" },
            { "NEPTUNE", " 5.48 km/s", "1.17 g" },
        };

        gui::gl::Font& fnt = _text.getFont(0);
        fnt.reset();

        const td::ColorID clr = td::ColorID::Yellow;
        gui::gl::Point2d pt(12.0f, 10.0f);
        gui::gl::Size s;

        if (_planetView >= 1 && _planetView <= 8)
        {
            const PlanetInfo& p = planets[_planetView - 1];
            char line[64];
            s = fnt.addString(pt, p.name, 0, clr);               pt.y += s.height;
            td::format(line, "Orbital speed: %s", p.speed);
            s = fnt.addString(pt, line, 0, clr);                  pt.y += s.height;
            td::format(line, "Gravity: %s", p.gravity);
            fnt.addString(pt, line, 0, clr);
        }
        else
        {
            s = fnt.addString(pt, "SOLAR SYSTEM",  clr); pt.y += s.height;
            s = fnt.addString(pt, "STARS: 1 (SUN)", clr); pt.y += s.height;
            fnt.addString(pt, "PLANETS: 8", clr);
        }

        const char* mode = (_planetView >= 1 && _planetView <= 8) ? "PLANET CAM"
                           : (_freeCam ? "FREE CAM" : "STATIC CAM");
        gui::gl::Font::Format right;
        right.colorID    = clr;
        right.hAlignment = td::HAlignment::Right;
        gui::gl::Point2d rp((float)_frameSize.width - 12.0f, 10.0f);
        fnt.addString(rp, mode, 0, right);
    }

protected:
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("INFO! Used OpenGL %d.%d", major, minor);

        if (!_bodyProgram.link(":bodyVS", ":bodyFS"))
        {
            mu::dbgLog("ERROR! Cannot link body program");
            assert(false); return;
        }
        if (!_ringProgram.link(":ringVS", ":ringFS"))
        {
            mu::dbgLog("ERROR! Cannot link ring program");
            assert(false); return;
        }

        _sphereBuffer.init(24, 8192, 65536, {gui::gl::DataType::vec3, gui::gl::DataType::vec2});
        _orbitBuffer.init(72, 512, 0, {gui::gl::DataType::vec3});

        _textures.reserve(TEX_COUNT);
        _textures.append({":texSun", ":texMercury", ":texVenus", ":texEarth", ":texMoon",
                          ":texMars", ":texJupiter", ":texSaturn", ":texUranus", ":texNeptune"});
        _textures.appendCubeMap({":sfLeft", ":sfFront", ":sfRight", ":sfBack", ":sfTop", ":sfBottom"});
        _textures.appendCubeMap({":blLeft", ":blFront", ":blRight", ":blBack", ":blTop", ":blBottom"});
        assert(_textures.size() == TEX_COUNT);

        auto* sunMvp = _sphereBuffer.createCommand();
        sunMvp->createMVPSetter(&_mSun);
        _sphereBuffer.encode(sunMvp);

        gui::gl::Command* sunDraw = _sphereBuffer.addSphere(1.0f, 64, 32, gui::gl::Texture::Type::Regular2D);
        td::UINT4 nSphereIdx = (td::UINT4)_sphereBuffer.getNoOfIndices();
        sunDraw->setTexture(gui::gl::Textures::Unit::T0, (td::INT1)TEX_SUN);
        _sphereBuffer.encode(sunDraw);

        auto encodeBody = [&](glm::mat4& mBody, td::BYTE texIdx)
        {
            auto* mvp = _sphereBuffer.createCommand();
            mvp->createMVPSetter(&mBody);
            _sphereBuffer.encode(mvp);

            auto* draw = _sphereBuffer.createCommand();
            draw->createDrawElements(gui::gl::Primitive::Triangles, 0, nSphereIdx);
            draw->setTexture(gui::gl::Textures::Unit::T0, (td::INT1)texIdx);
            _sphereBuffer.encode(draw);
        };

        encodeBody(_mMercury, TEX_MERCURY);
        encodeBody(_mVenus,   TEX_VENUS);
        encodeBody(_mEarth,   TEX_EARTH);
        encodeBody(_mMoon,    TEX_MOON);
        encodeBody(_mMars,    TEX_MARS);
        encodeBody(_mJupiter, TEX_JUPITER);
        encodeBody(_mSaturn,  TEX_SATURN);
        encodeBody(_mUranus,  TEX_URANUS);
        encodeBody(_mNeptune, TEX_NEPTUNE);

        for (int i = 0; i < ORBIT_PTS; ++i)
        {
            float a = (float)i / (float)ORBIT_PTS * 2.0f * (float)M_PI;
            float v[3] = { std::sinf(a), 0.0f, std::cosf(a) };
            _orbitBuffer.appendVertexUnits(v, 3);
        }

        _ringMtx.resize(34);
        for (int i = 0; i < 34; ++i)
        {
            auto* mvp = _orbitBuffer.createCommand();
            mvp->createMVPSetter(&_ringMtx[i]);
            _orbitBuffer.encode(mvp);

            auto* draw = _orbitBuffer.createCommand();
            draw->createDrawArrays(gui::gl::Primitive::LineLoop, 0, ORBIT_PTS);
            draw->setColor(i < 9 ? td::ColorID::White : td::ColorID::PaleGoldenTod);
            _orbitBuffer.encode(draw);
        }

        if (!_sphereBuffer.commit())
            mu::dbgLog("ERROR! sphereBuffer commit failed");
        if (!_orbitBuffer.commit())
            mu::dbgLog("ERROR! orbitBuffer commit failed");

        _skyBoxStarfield.init(&_skyVP, _textures.getID(TEX_CM_STARFIELD), gui::gl::Textures::Unit::T1);
        _skyBoxStarfield.commit(5000.0f);
        _skyBoxBlue.init(&_skyVP, _textures.getID(TEX_CM_BLUE), gui::gl::Textures::Unit::T1);
        _skyBoxBlue.commit(5000.0f);

        _bodyProgram.setBuffer(&_sphereBuffer);
        _bodyProgram.setTextures(&_textures);
        _ringProgram.setBuffer(&_orbitBuffer);

        gui::Size sz;
        getSize(sz);
        if (sz.width < 1) { sz.width = 1200; sz.height = 800; }
        _text.init(sz, gui::gl::Textures::Unit::T15);
        _text.allocFonts(1);
        if (!_text.loadFont(0, ":lucida"))
            mu::dbgLog("ERROR! HUD font ':lucida' failed to load");
        _text.commit();

        // Initialise the projection up front so the first frame (before the first
        // onResize) renders with a valid matrix instead of a transient garbage one.
        _perspectiveMatrix = glm::perspective(
            glm::radians(45.0f), (float)sz.width / (float)sz.height, 0.1f, 20000.0f);

        setFocus();
    }

    bool prepareNextFrame() override
    {
        float t = getTime();
        _deltaTime = t - _lastFrame;
        _lastFrame = t;

        glm::mat4 view(1.0f);

        if (_planetView >= 1 && _planetView <= 8)
        {
            static const float specs[8][5] = {
                {1.00f,  2.0f,  3.5f, 50.0f, 1.3f},
                {0.75f,  3.0f,  4.5f, 50.0f, 1.2f},
                {0.55f,  4.0f,  5.5f, 50.0f, 1.2f},
                {0.35f,  5.0f,  6.0f, 20.0f, 1.2f},
                {0.20f,  6.0f,  7.5f, 50.0f, 1.3f},
                {0.15f,  7.0f,  8.5f, 50.0f, 1.3f},
                {0.10f,  8.0f,  9.5f, 50.0f, 1.3f},
                {0.08f,  9.0f, 10.5f, 50.0f, 1.3f},
            };
            const float* s = specs[_planetView - 1];
            float phase = t * _planetSpeed * s[0];
            glm::vec3 target(std::sin(phase) * 100.0f * s[1] * 1.3f, 0.0f,
                             std::cos(phase) * 100.0f * s[1] * 1.3f);
            glm::vec3 eye   (std::sin(phase) * 100.0f * s[2] * s[4], s[3],
                             std::cos(phase) * 100.0f * s[2] * s[4]);
            view = glm::lookAt(eye, target, glm::vec3(0, 1, 0));
        }
        else
        {
            if (_freeCam)
            {
                gui::Point cur;
                bool inFocus = getCursorPosition(cur);
                moveCursorToCenter();
#ifdef __APPLE__
                CGAssociateMouseAndMouseCursorPosition(true);
#endif
                gui::Point center;
                bool inFocusAfter = getCursorPosition(center);

                if (inFocus && inFocusAfter && !_firstMouseFrame)
                {
                    float dx = (float)(cur.x - center.x);
                    float dy = (float)(center.y - cur.y);
                    _camYaw   += dx * MOUSE_SENS;
                    _camPitch += dy * MOUSE_SENS;
                    if (_camPitch >  89.0f) _camPitch =  89.0f;
                    if (_camPitch < -89.0f) _camPitch = -89.0f;
                }
                _firstMouseFrame = false;
            }

            glm::vec3 front = camFront();
            glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
            float v = _moveSpeed * _deltaTime;
            if (_keys['w'] || _keys['W']) _flyPos += front * v;
            if (_keys['s'] || _keys['S']) _flyPos -= front * v;
            if (_keys['a'] || _keys['A']) _flyPos -= right * v;
            if (_keys['d'] || _keys['D']) _flyPos += right * v;

            view = glm::lookAt(_flyPos, _flyPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        _vp = _perspectiveMatrix * view;

        glm::mat4 skyView = view;
        skyView[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        _skyVP = _perspectiveMatrix * skyView * sceneMat();

        {
            glm::mat4 m(1.0f);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, t * glm::radians(23.5f) * 0.25f, glm::vec3(0, 0, 1));
            _mSun = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed)             * 100.0f * 2.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed)             * 100.0f * 2.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, t * glm::radians(-90.0f) * 0.05f, glm::vec3(0, 0, 1));
            _mMercury = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.75f)     * 100.0f * 3.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.75f)     * 100.0f * 3.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-132.5f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-132.5f) * 0.012f, glm::vec3(0, 0, 1));
            _mVenus = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(12.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.55f)     * 100.0f * 4.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.55f)     * 100.0f * 4.0f * 1.3f;
            _earthPos = glm::vec3(xx, 0.0f, zz);
            glm::mat4 m = sceneMat();
            m = glm::translate(m, _earthPos);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-33.25f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-33.25f) * 2.0f, glm::vec3(0, 0, 1));
            _mEarth = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(11.8f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 67.55f)    * 100.0f * 0.5f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 67.55f)    * 100.0f * 0.5f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, _earthPos);
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-32.4f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-32.4f) * 3.1f, glm::vec3(0, 0, 1));
            _mMoon = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(5.5f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.35f)     * 100.0f * 5.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.35f)     * 100.0f * 5.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-32.4f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-32.4f) * 2.1f, glm::vec3(0, 0, 1));
            _mMars = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.2f)      * 100.0f * 6.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.2f)      * 100.0f * 6.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-23.5f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-23.5f) * 4.5f, glm::vec3(0, 0, 1));
            _mJupiter = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(40.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.15f)     * 100.0f * 7.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.15f)     * 100.0f * 7.0f * 1.3f;
            _saturnPos = glm::vec3(xx, 0.0f, zz);
            glm::mat4 m = sceneMat();
            m = glm::translate(m, _saturnPos);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-34.7f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-34.7f) * 4.48f, glm::vec3(0, 0, 1));
            _mSaturn = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(37.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.1f)      * 100.0f * 8.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.1f)      * 100.0f * 8.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-99.0f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-99.0f) * 4.5f, glm::vec3(0, 0, 1));
            _mUranus = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
        }

        {
            float xx = std::sin(t * _planetSpeed * 0.08f)     * 100.0f * 9.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.08f)     * 100.0f * 9.0f * 1.3f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-30.2f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-30.2f) * 4.0f, glm::vec3(0, 0, 1));
            _mNeptune = _vp * m * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
        }

        for (int i = 0; i < 8; ++i)
        {
            float s = (float)(i + 2) * 1.3f * 100.0f;
            glm::mat4 m = sceneMat();
            m = glm::scale(m, glm::vec3(s));
            _ringMtx[i] = _vp * m;
        }

        {
            float s = 0.5f * 1.3f * 100.0f;
            glm::mat4 m = sceneMat();
            m = glm::translate(m, _earthPos);
            m = glm::scale(m, glm::vec3(s));
            _ringMtx[8] = _vp * m;
        }

        {
            float rr = 0.55f * 100.0f;
            for (int i = 0; i < 25; ++i)
            {
                glm::mat4 m = sceneMat();
                m = glm::translate(m, _saturnPos);
                m = glm::rotate(m, glm::radians(30.0f), glm::vec3(0, 0, 1));
                m = glm::scale(m, glm::vec3(rr));
                _ringMtx[9 + i] = _vp * m;
                if (i == 15) rr += 3.0f;
                else         rr += 1.0f;
            }
        }

        buildHUD();
        return true;
    }

    void onDraw(const gui::Rect&) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        (_skyBoxExtra ? _skyBoxBlue : _skyBoxStarfield).render();

        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::disable(gui::gl::Context::Flag::CullFace);

        _bodyProgram.activate();
        _bodyProgram.execute();
        _bodyProgram.deActivate();

        _ringProgram.activate();
        _ringProgram.execute();
        _ringProgram.deActivate();

        _text.render();
    }

    void onResize(const gui::Size& newSize) override
    {
        if (newSize.width == 0 || newSize.height == 0) return;
        CameraView::onResize(newSize);
        _perspectiveMatrix = glm::perspective(
            glm::radians(45.0f),
            (float)newSize.width / (float)newSize.height,
            0.1f, 20000.0f);
        _text.setFrameSize(newSize);
    }

    void enterFreeCam()
    {
        _planetView = 0;
        _freeCam = true;
        _firstMouseFrame = true;
        setFocus();
        showCursor(false);
    }

    void exitFreeCam()
    {
        _planetView = 0;
        _freeCam = false;
        _firstMouseFrame = true;
        showCursor(true);
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.getVirtual() == gui::Key::Virtual::F1)
        {
            enterFreeCam();
            return true;
        }
        if (key.getCode() == ' ' || (key.isASCII() && key.getChar() == ' '))
        {
            exitFreeCam();
            return true;
        }
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch >= 0) _keys[(unsigned char)ch] = true;

            if (ch == 'e' || ch == 'E')
            {
                _skyBoxExtra = !_skyBoxExtra;
                return true;
            }
            if (ch >= '1' && ch <= '8')
            {
                _planetView = ch - '0';
                _freeCam = false;
                _firstMouseFrame = true;
                showCursor(true);
                return true;
            }
            return true;
        }
        return false;
    }

    bool onKeyReleased(const gui::Key& key) override
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch >= 0) _keys[(unsigned char)ch] = false;
            return true;
        }
        return false;
    }

    void onSecondaryButtonPressed(const gui::InputDevice&) override
    {
        _onRotate = true;
        _firstDragPt = true;
    }

    void onSecondaryButtonReleased(const gui::InputDevice&) override
    {
        _onRotate = false;
    }

    void onCursorMoved(const gui::InputDevice& dev) override
    {
        if (_onRotate && !_freeCam && _planetView == 0)
        {
            auto pt = dev.getFramePoint();
            if (!_firstDragPt)
            {
                float dx = (float)pt.x - _lastDragX;
                float dy = (float)pt.y - _lastDragY;
                _sceneRotateX += dx * 0.1f;
                _sceneRotateY -= dy * 0.1f;
            }
            _lastDragX = (float)pt.x;
            _lastDragY = (float)pt.y;
            _firstDragPt = false;
        }
    }

public:
    SolarSystemView()
    : _camera(glm::vec3(0.0f, 1200.0f, 2500.0f), -90.0f, -25.6f)
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes,
                                    gui::gl::DepthBuffer::Size::B2);
        gui::gl::View::createContext(reqContext, {
            gui::InputDevice::Event::Keyboard,
            gui::InputDevice::Event::PrimaryClicks,
            gui::InputDevice::Event::SecondaryClicks,
            gui::InputDevice::Event::CursorMove,
            gui::InputDevice::Event::Zoom
        });
        setCamera(&_camera);
        registerForFocusEvents();
    }

    ~SolarSystemView()
    {
        makeCurrentContext();
    }
};
