#pragma once

#include <gui/gl/View.h>
#include <gui/gl/Text.h>
#include <gui/Key.h>
#include <gui/InputDevice.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"

#include <chrono>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>

#include "stb_image.h"

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

class SolarSystemView : public gui::gl::View
{
    Camera _camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 _origin{0.0f, 0.0f, 0.0f};
    bool _keys[1024] = {false};
    bool _onRotate = false;
    bool _onFreeCam = true;
    bool _skyBoxExtra = false;
    int _planetView = 0;
    float _planetSpeed = 0.1f;
    float _sceneRotateY = 0.0f;
    float _sceneRotateX = 0.0f;
    bool _firstMouseFrame = true;
    float _lastMouseX = 0.0f;
    float _lastMouseY = 0.0f;

    float _screenW = 1200.0f;
    float _screenH = 800.0f;

    std::chrono::steady_clock::time_point _startTime = std::chrono::steady_clock::now();
    float _lastFrame = 0.0f;
    float _deltaTime = 0.0f;

    GLuint _simpleProgID = 0;
    GLuint _skyboxProgID = 0;

    GLuint _skyboxVAO = 0;
    GLuint _skyboxVBO = 0;

    GLuint _orbVAO = 0;
    GLuint _orbVBO = 0;
    int _orbVertexCount = 0;

    GLuint _texSun = 0;
    GLuint _texMercury = 0;
    GLuint _texVenus = 0;
    GLuint _texEarth = 0;
    GLuint _texMoon = 0;
    GLuint _texMars = 0;
    GLuint _texJupiter = 0;
    GLuint _texSaturn = 0;
    GLuint _texUranus = 0;
    GLuint _texNeptune = 0;
    GLuint _texSaturnRing = 0;
    GLuint _texEarthClouds = 0;
    GLuint _cubemapStarfield = 0;
    GLuint _cubemapBlue = 0;

    Sphere* _sun = nullptr;
    Sphere* _mercury = nullptr;
    Sphere* _venus = nullptr;
    Sphere* _earth = nullptr;
    Sphere* _mars = nullptr;
    Sphere* _jupiter = nullptr;
    Sphere* _saturn = nullptr;
    Sphere* _uranus = nullptr;
    Sphere* _neptune = nullptr;
    Sphere* _moon = nullptr;

    gui::gl::Text _text;

private:
    float getTime() const
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = now - _startTime;
        return dt.count();
    }

    static unsigned int loadTextureFromFile(const td::String& path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        int width = 0, height = 0, nrComponents = 0;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format = GL_RGB;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            mu::dbgLog("Texture failed to load at path: %s", path.c_str());
        }
        return textureID;
    }

    static unsigned int loadCubemapFromFiles(const std::vector<td::String>& faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                mu::dbgLog("Cubemap face failed to load: %s", faces[i].c_str());
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void buildSkyboxGeometry()
    {
        static const float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &_skyboxVAO);
        glGenBuffers(1, &_skyboxVBO);
        glBindVertexArray(_skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    void buildOrbitGeometry()
    {
        std::vector<float> orbVert;
        for (int i = 0; i < 2000; i++)
        {
            float angl = (float)(M_PI / 2.0 - i * (M_PI / 1000.0));
            float xx = std::sin(angl) * 100.0f;
            float zz = std::cos(angl) * 100.0f;
            orbVert.push_back(xx);
            orbVert.push_back(0.0f);
            orbVert.push_back(zz);
        }
        _orbVertexCount = (int)(orbVert.size() / 3);

        glGenVertexArrays(1, &_orbVAO);
        glGenBuffers(1, &_orbVBO);
        glBindVertexArray(_orbVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _orbVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbVert.size(), orbVert.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void loadAllTextures()
    {
        
//        _texEarth        = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/earth2k.jpg");
        _texEarth        = loadTextureFromFile(gui::getResFileName("earth2k"));
//        _texSun          = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_sun.jpg");
//        _texMoon         = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_moon.jpg");
//        _texMercury      = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_mercury.jpg");
//        _texVenus        = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_mercury.jpg");
//        _texMars         = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_mars.jpg");
//        _texJupiter      = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_jupiter.jpg");
//        _texSaturn       = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_saturn.jpg");
//        _texUranus       = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_uranus.jpg");
//        _texNeptune      = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_neptune.jpg");
//        _texSaturnRing   = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/r.jpg");
//        _texEarthClouds  = loadTextureFromFile(SOLARSYSTEM_ASSETS_DIR "/planets/2k_earth_clouds.jpg");
        
        _texSun          = loadTextureFromFile(gui::getResFileName("2k_sun"));
        _texMoon         = loadTextureFromFile(gui::getResFileName("2k_moon"));
        _texMercury      = loadTextureFromFile(gui::getResFileName("2k_mercury"));
        _texVenus        = loadTextureFromFile(gui::getResFileName("2k_venus"));
        _texMars         = loadTextureFromFile(gui::getResFileName("2k_mars"));
        _texJupiter      = loadTextureFromFile(gui::getResFileName("2k_jupiter"));
        _texSaturn       = loadTextureFromFile(gui::getResFileName("2k_saturn"));
        _texUranus       = loadTextureFromFile(gui::getResFileName("2k_uranus"));
        _texNeptune      = loadTextureFromFile(gui::getResFileName("2k_neptune"));
        _texSaturnRing   = loadTextureFromFile(gui::getResFileName("saturn_ring"));
        _texEarthClouds  = loadTextureFromFile(gui::getResFileName("2k_earth_clouds"));

//        std::vector<td::String> faces = {
//            gui::getResFileName("starfield_rt"),
////            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_rt.tga",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_lf.tga",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_up.tga",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_dn.tga",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_ft.tga",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/starfield/starfield_bk.tga",
//        };
//        std::vector<td::String> facesExtra = {
//            gui::getResFileName("bkg1_right"),
////            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_right.png",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_left.png",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_top.png",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_bot.png",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_front.png",
//            SOLARSYSTEM_ASSETS_DIR "/skybox/blue/bkg1_back.png",
//        };
        std::vector<td::String> faces = {
            gui::getResFileName("starfield_rt"),
            gui::getResFileName("starfield_lf"),
            gui::getResFileName("starfield_up"),
            gui::getResFileName("starfield_dn"),
            gui::getResFileName("starfield_ft"),
            gui::getResFileName("starfield_bk"),
        };
        std::vector<td::String> facesExtra = {
            gui::getResFileName("bkg1_right"),
            gui::getResFileName("bkg1_left"),
            gui::getResFileName("bkg1_top"),
            gui::getResFileName("bkg1_bot"),
            gui::getResFileName("bkg1_front"),
            gui::getResFileName("bkg1_back"),
        };
        
        _cubemapStarfield = loadCubemapFromFiles(faces);
        _cubemapBlue      = loadCubemapFromFiles(facesExtra);
    }

    void applyKeyboardMovement()
    {
        if (_keys['W'] || _keys['w']) _camera.ProcessKeyboard(FORWARD,  _deltaTime);
        if (_keys['S'] || _keys['s']) _camera.ProcessKeyboard(BACKWARD, _deltaTime);
        if (_keys['A'] || _keys['a']) _camera.ProcessKeyboard(LEFT,     _deltaTime);
        if (_keys['D'] || _keys['d']) _camera.ProcessKeyboard(RIGHT,    _deltaTime);
    }

    void buildHUD()
    {
        struct PlanetData { const char* name; const char* speed; const char* gravity; };
        static const PlanetData planets[8] = {
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
            const PlanetData& p = planets[_planetView - 1];
            char line[64];
            s = fnt.addString(pt, p.name, 0, clr);                        pt.y += s.height;
            td::format(line, "Orbital speed: %s", p.speed);
            s = fnt.addString(pt, line, 0, clr);                           pt.y += s.height;
            td::format(line, "Gravity: %s", p.gravity);
            s = fnt.addString(pt, line, 0, clr);
        }
        else
        {
            s = fnt.addString(pt, "SOLAR SYSTEM", clr);   pt.y += s.height;
            s = fnt.addString(pt, "STARS: 1 (SUN)", clr); pt.y += s.height;
            s = fnt.addString(pt, "PLANETS: 8", clr);
        }

        const char* mode = (_planetView >= 1 && _planetView <= 8) ? "PLANET CAM"
                           : (_camera.FreeCam ? "FREE CAM" : "STATIC CAM");
        gui::gl::Font::Format right;
        right.colorID    = clr;
        right.hAlignment = td::HAlignment::Right;
        gui::gl::Point2d rp(_screenW - 12.0f, 10.0f);
        fnt.addString(rp, mode, 0, right);
    }

protected:
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);

        _simpleProgID = linkProgram(":simpleVS", ":simpleFS");
        _skyboxProgID = linkProgram(":skyboxVS", ":skyboxFS");
        if (_simpleProgID == 0 || _skyboxProgID == 0)
        {
            mu::dbgLog("ERROR! Cannot link solar-system shaders");
            assert(false);
            return;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        buildSkyboxGeometry();
        buildOrbitGeometry();
        loadAllTextures();

        _sun     = new Sphere(100.0f, 36 * 5, 18 * 5);
        _mercury = new Sphere(10.0f, 36, 18);
        _venus   = new Sphere(12.0f, 36, 18);
        _earth   = new Sphere(11.8f, 36, 18);
        _mars    = new Sphere(8.0f, 36, 18);
        _jupiter = new Sphere(40.0f, 36, 18);
        _saturn  = new Sphere(37.0f, 36, 18);
        _uranus  = new Sphere(30.0f, 36, 18);
        _neptune = new Sphere(30.0f, 36, 19);
        _moon    = new Sphere(5.5f, 36, 18);

        _camera.Position = glm::vec3(0.0f, 1200.0f, -2500.0f);
        _camera.Yaw   = 90.0f;
        _camera.Pitch = -25.0f;
        _camera.ProcessMouseMovement(0.0f, 0.0f);
        _camera.FreeCam = false;
        _onFreeCam = true;

        setFocus();

        gui::Size sz;
        getSize(sz);
        if (sz.width < 1) { sz.width = 1200; sz.height = 800; }
        _text.init(sz, gui::gl::Textures::Unit::T15);
        _text.allocFonts(1);
        if (!_text.loadFont(0, ":lucida"))
            mu::dbgLog("ERROR! HUD font ':lucida' failed to load");
        _text.commit();
    }

    bool prepareNextFrame() override
    {
        float currentFrame = getTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        if (!_camera.FreeCam)
        {
            float y = _camera.Position.y;
            if (y < 200.0f && y > 200.0f)        _camera.MovementSpeed = 300.0f;
            if (y < 125.0f && y >  70.0f)        _camera.MovementSpeed = 200.0f;
            if (y <  70.0f && y >  50.0f)        _camera.MovementSpeed = 100.0f;
            if (y > 200.0f && y < 400.0f)        _camera.MovementSpeed = 400.0f;
            if (y > 125.0f && y < 200.0f)        _camera.MovementSpeed = 300.0f;
            if (y >  70.0f && y < 125.0f)        _camera.MovementSpeed = 200.0f;
        }

        applyKeyboardMovement();

        if (_camera.FreeCam)
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
                _camera.ProcessMouseMovement(dx, dy, false);
            }
            _firstMouseFrame = false;
        }

        if (!_onFreeCam)
        {
            _sceneRotateY = 0.0f;
            _sceneRotateX = 0.0f;
        }

        buildHUD();
        return true;
    }

    void onDraw(const gui::Rect& rect) override
    {
        _screenW = (float)rect.width();
        _screenH = (float)rect.height();
        if (_screenW < 1.0f) _screenW = 1.0f;
        if (_screenH < 1.0f) _screenH = 1.0f;

        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader SimpleShader(_simpleProgID);
        Shader SkyboxShader(_skyboxProgID);

        SimpleShader.Use();

        glm::mat4 model(1.0f);
        glm::mat4 view = _camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom),
                                                _screenW / _screenH, 0.1f, 10000.0f);
        SimpleShader.setMat4("model", model);
        SimpleShader.setMat4("view", view);
        SimpleShader.setMat4("projection", projection);

        float t = getTime();
        glm::vec3 PlanetsPositions[9];

        if (_planetView > 0 && _planetView <= 8)
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
            glm::vec3 target(std::sin(phase) * 100.0f * s[1] * 1.3f,
                             0.0f,
                             std::cos(phase) * 100.0f * s[1] * 1.3f);
            glm::vec3 viewPos(std::sin(phase) * 100.0f * s[2] * s[4],
                              s[3],
                              std::cos(phase) * 100.0f * s[2] * s[4]);
            view = glm::lookAt(viewPos, target, glm::vec3(0, 1, 0));
            SimpleShader.setMat4("view", view);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texSun);
        {
            glm::mat4 m(1.0f);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, t * glm::radians(23.5f) * 0.25f, glm::vec3(0, 0, 1));
            m = glm::translate(m, _origin);
            SimpleShader.setMat4("model", m);
            _sun->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texMercury);
        {
            float xx = std::sin(t * _planetSpeed) * 100.0f * 2.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed) * 100.0f * 2.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[0] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, t * glm::radians(-90.0f) * 0.05f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _mercury->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texVenus);
        {
            float xx = std::sin(t * _planetSpeed * 0.75f) * 100.0f * 3.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.75f) * 100.0f * 3.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[1] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-132.5f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-132.5f) * 0.012f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _venus->Draw();
        }

        glm::vec3 EarthPoint;
        glBindTexture(GL_TEXTURE_2D, _texEarth);
        {
            float xx = std::sin(t * _planetSpeed * 0.55f) * 100.0f * 4.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.55f) * 100.0f * 4.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            EarthPoint = glm::vec3(xx, 0.0f, zz);
            PlanetsPositions[2] = EarthPoint;
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-33.25f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-33.25f) * 2.0f, glm::vec3(0, 0, 1));
            _camera.LookAtPos = glm::vec3(m[3][0], m[3][1], m[3][2]);
            SimpleShader.setMat4("model", m);
            _earth->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texMoon);
        {
            float xx = std::sin(t * _planetSpeed * 67.55f) * 100.0f * 0.5f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 67.55f) * 100.0f * 0.5f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, EarthPoint);
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-32.4f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-32.4f) * 3.1f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _moon->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texMars);
        {
            float xx = std::sin(t * _planetSpeed * 0.35f) * 100.0f * 5.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.35f) * 100.0f * 5.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[3] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-32.4f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-32.4f) * 2.1f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _mars->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texJupiter);
        {
            float xx = std::sin(t * _planetSpeed * 0.2f) * 100.0f * 6.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.2f) * 100.0f * 6.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[4] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-23.5f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-23.5f) * 4.5f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _jupiter->Draw();
        }

        glm::vec3 SaturnPoint;
        glBindTexture(GL_TEXTURE_2D, _texSaturn);
        {
            float xx = std::sin(t * _planetSpeed * 0.15f) * 100.0f * 7.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.15f) * 100.0f * 7.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            SaturnPoint = glm::vec3(xx, 0.0f, zz);
            PlanetsPositions[5] = SaturnPoint;
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-34.7f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-34.7f) * 4.48f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _saturn->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texUranus);
        {
            float xx = std::sin(t * _planetSpeed * 0.1f) * 100.0f * 8.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.1f) * 100.0f * 8.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[6] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-99.0f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-99.0f) * 4.5f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _uranus->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texNeptune);
        {
            float xx = std::sin(t * _planetSpeed * 0.08f) * 100.0f * 9.0f * 1.3f;
            float zz = std::cos(t * _planetSpeed * 0.08f) * 100.0f * 9.0f * 1.3f;
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, glm::vec3(xx, 0.0f, zz));
            PlanetsPositions[7] = glm::vec3(xx, 0.0f, zz);
            m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(-30.2f), glm::vec3(0, 1, 0));
            m = glm::rotate(m, t * glm::radians(-30.2f) * 4.0f, glm::vec3(0, 0, 1));
            SimpleShader.setMat4("model", m);
            _neptune->Draw();
        }

        glBindTexture(GL_TEXTURE_2D, _texVenus);
        glBindVertexArray(_orbVAO);
        glLineWidth(1.0f);
        for (float i = 2; i < 10; i += 1.0f)
        {
            glm::mat4 m(1.0f);
            m = glm::translate(m, _origin);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::scale(m, glm::vec3(i * 1.3f, i * 1.3f, i * 1.3f));
            SimpleShader.setMat4("model", m);
            glDrawArrays(GL_LINE_LOOP, 0, _orbVertexCount);
        }
        {
            glm::mat4 m(1.0f);
            m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
            m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
            m = glm::translate(m, EarthPoint);
            m = glm::scale(m, glm::vec3(0.5f * 1.3f, 0.5f * 1.3f, 0.5f * 1.3f));
            SimpleShader.setMat4("model", m);
            glDrawArrays(GL_LINE_LOOP, 0, _orbVertexCount);
        }

        glBindTexture(GL_TEXTURE_2D, _texSaturnRing);
        glLineWidth(2.0f);
        {
            float rr = 0.55f;
            for (int i = 0; i < 25; i++)
            {
                glm::mat4 m(1.0f);
                m = glm::rotate(m, glm::radians(_sceneRotateY), glm::vec3(1, 0, 0));
                m = glm::rotate(m, glm::radians(_sceneRotateX), glm::vec3(0, 0, 1));
                m = glm::translate(m, SaturnPoint);
                m = glm::rotate(m, glm::radians(30.0f), glm::vec3(0, 0, 1));
                m = glm::scale(m, glm::vec3(rr, rr, rr));
                SimpleShader.setMat4("model", m);
                glDrawArrays(GL_LINE_LOOP, 0, _orbVertexCount);
                if (i == 15) rr += 0.030f;
                else         rr += 0.01f;
            }
        }
        glBindVertexArray(0);

        glDepthFunc(GL_LEQUAL);
        SkyboxShader.Use();
        glm::mat4 skyView = glm::mat4(glm::mat3(_camera.GetViewMatrix()));
        SkyboxShader.setMat4("view", skyView);
        SkyboxShader.setMat4("projection", projection);
        glBindVertexArray(_skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skyBoxExtra ? _cubemapBlue : _cubemapStarfield);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        _text.render();
    }

    void onResize(const gui::Size& newSize) override
    {
        if (newSize.width == 0 || newSize.height == 0)
            return;
        gui::gl::View::onResize(newSize);
        _screenW = (float)newSize.width;
        _screenH = (float)newSize.height;
        _text.setFrameSize(newSize);
    }

    void enterFreeCam()
    {
        _planetView = 0;
        _onFreeCam = false;
        _camera.FreeCam = true;
        _firstMouseFrame = true;
        setFocus();
        showCursor(false);
    }

    void exitFreeCam()
    {
        _planetView = 0;
        _onFreeCam = true;
        _camera.FreeCam = false;
        _camera.Position = glm::vec3(0.0f, 1200.0f, -2500.0f);
        _camera.Yaw   = 90.0f;
        _camera.Pitch = -25.0f;
        _camera.ProcessMouseMovement(0.0f, 0.0f);
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
        if (key.getVirtual() == gui::Key::Virtual::Esc)
        {
            return true;
        }
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch >= 0) _keys[(unsigned char)ch] = true;

            if (ch == 'E' || ch == 'e') { _skyBoxExtra = !_skyBoxExtra; return true; }
            if (ch == 'P' || ch == 'p') { return true; }
            if (ch >= '1' && ch <= '8')
            {
                _planetView = ch - '0';
                _onFreeCam = false;
                _camera.FreeCam = false;
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

    void onPrimaryButtonPressed(const gui::InputDevice&) override
    {
    }

    void onSecondaryButtonPressed(const gui::InputDevice&) override
    {
        if (_onFreeCam && !_camera.FreeCam) _onRotate = true;
    }

    void onSecondaryButtonReleased(const gui::InputDevice&) override
    {
        _onRotate = false;
    }

    bool onScroll(const gui::InputDevice& inputDevice) override
    {
        auto d = inputDevice.getScrollDelta();
        _camera.ProcessMouseScroll((float)d.y);
        return true;
    }

    void onCursorMoved(const gui::InputDevice& inputDevice) override
    {
        auto pt = inputDevice.getFramePoint();
        if (_firstMouseFrame)
        {
            _lastMouseX = (float)pt.x;
            _lastMouseY = (float)pt.y;
            _firstMouseFrame = false;
            return;
        }
        float dx = (float)pt.x - _lastMouseX;
        float dy = _lastMouseY - (float)pt.y;
        _lastMouseX = (float)pt.x;
        _lastMouseY = (float)pt.y;

        if (!_camera.FreeCam && _onRotate)
        {
            _sceneRotateY += dy * 0.1f;
            _sceneRotateX += dx * 0.1f;
        }
    }

public:
    SolarSystemView()
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes,
                                    gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard,
                                   gui::InputDevice::Event::PrimaryClicks,
                                   gui::InputDevice::Event::SecondaryClicks,
                                   gui::InputDevice::Event::CursorMove,
                                   gui::InputDevice::Event::Zoom});
        registerForFocusEvents();
    }

    ~SolarSystemView()
    {
        makeCurrentContext();
        if (_simpleProgID) glDeleteProgram(_simpleProgID);
        if (_skyboxProgID) glDeleteProgram(_skyboxProgID);
        if (_orbVAO)     glDeleteVertexArrays(1, &_orbVAO);
        if (_orbVBO)     glDeleteBuffers(1, &_orbVBO);
        if (_skyboxVAO)  glDeleteVertexArrays(1, &_skyboxVAO);
        if (_skyboxVBO)  glDeleteBuffers(1, &_skyboxVBO);

        delete _sun; delete _mercury; delete _venus; delete _earth;
        delete _mars; delete _jupiter; delete _saturn; delete _uranus;
        delete _neptune; delete _moon;
    }
};
