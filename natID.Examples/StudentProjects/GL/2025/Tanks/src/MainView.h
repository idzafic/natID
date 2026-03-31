

#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "objloader.h"
#include "Camera.h"
#include "Stopwatch.h"
#include <gui/gl/Buffer.h>
#include <gui/Sound.h>
#include <chrono>
#include <iostream>
#include "Tenk.h"
#include "stb_easy_font.h"
#include <gui/gl/CameraView.h>
#include <gui/gl/SkyBox.h>
#include <gui/gl/Textures.h>
#include <gui/gl/Text.h>

#define NORMAL_xPLUS 1.0f, 0.0f, 0.0f
#define NORMAL_xMINUS -1.0f, 0.0f, 0.0f
#define NORMAL_yPLUS 0.0f, 1.0f, 0.0f
#define NORMAL_yMINUS 0.0f, -1.0f, 0.0f
#define NORMAL_zPLUS 0.0f, 0.0f, 1.0f
#define NORMAL_zMINUS 0.0f, 0.0f, -1.0f



class MainView : public gui::gl::CameraView
{
    gui::gl::SkyBox _skyBox;
    gui::gl::Textures _textures;
    glm::mat4 _cameraProjectionMatrix;

    MojaCamera _camera;
    Stopwatch _clock;
    gui::Size _size;

    td::UINT4 _mainPassProgramID;

    GLuint _vertexbuffer;
    GLuint _elementbuffer;
    GLuint _uvbuffer;
    GLuint _normalbuffer;

    gui::Texture _textHouse;

    gui::gl::Text _text;


    GLuint _textureMain;
    // Get a handle for our "myTextureSampler" uniform
    GLuint _textureIDInShader;


    // Get a handle for our "MVP" uniform
    GLuint _matrixID;

    td::UINT4 _indicesCnt;

    bool _cameraControl = true;
    float naprijed = 0.0f;

    bool idiNaprijed = false;
    bool idiNazad = false;

    bool pomjeriTenk1 = false;

    //float TranslacijaTenka = 0;

    bool pomjeriTenk1R = false;

    bool rotirajTenkD = false;

    bool rotirajTenkL = false;

   // float rotacijaTenka = 0;

    float rotacijaCijeviYaw = 0;


    float rotacijaCijeviPitch = 0;
     
    Tenk _tenk1;

    Tenk _tenk2;

    bool activePlayer = false;

    long int animStep = 0;
    int dodaj = 1;

    float orbitYaw = 0.0f;
    float orbitPitch = 20.0f; 
    float orbitRadius = 17.0f;


    std::vector<glm::vec3> _forbiddenPoints;

    gui::Sound _zvukMotora;

    gui::Sound _zvukFire;

    gui::Sound _zvukThrottle;

    std::chrono::steady_clock::time_point _startTimeMotor;

    std::chrono::steady_clock::time_point _startTimeThrottle;

    std::chrono::steady_clock::time_point _startTimePlayer;

    std::chrono::steady_clock::time_point now;

    int preostaloVrijeme = 0;

    Tenk* _aktivniTenk = NULL;

    Tenk* _pasivniTenk = NULL;

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;

    bool _udarioUPrepreku = false;

private:
    bool setupShaders()
    {
        _mainPassProgramID = linkProgram(":mainV", ":mainF");


        assert(_mainPassProgramID > 0);

        return true;
    }


    void setup()
    {

        _textures.reserve(1);
        _textures.appendCubeMap({ ":cm3Left", ":cm3Front", ":cm3Right", ":cm3Back", ":cm3Top", ":cm3Bottom" }); //sand

        assert(_textures.size() == 1);

        td::UINT4 textCubeMap = _textures.getID(0);
        _skyBox.init(&_cameraProjectionMatrix, textCubeMap, gui::gl::Textures::Unit::T3);
        _skyBox.commit(10000.0f);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);


        // Load the texture
        fo::fs::path homePath;
        mu::getHomePath(homePath);


        _textureMain = loadJPG(_textHouse);


        // Read .obj file
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        td::String roomWalls = gui::getResFileName(":cityObj");
        bool res = loadOBJ(roomWalls.c_str(), false, vertices, uvs, normals);

        _tenk1.postaviHeightMapu(vertices);
        _tenk2.postaviHeightMapu(vertices);

        indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
        _indicesCnt = (td::UINT4)indices.size();



        //gdje tenk ne moze ici : 

        for (const auto& v : indexed_vertices) {
            if (v.y > 1.0f) { // sve iznad ceste
                _forbiddenPoints.push_back(v);
            }
        }


        glGenBuffers(1, &_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


        gui::Size sz;
        getSize(sz);
        _text.init(sz, gui::gl::Textures::Unit::T15);

        _text.allocFonts(3);

        if (!_text.loadFont(0, ":korataki"))
                return;
        if (!_text.loadFont(1, ":vinq")) // vinq
            return;

        if (!_text.loadFont(2, ":korataki"))
            return;
    }


protected:
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _camera.setViewportSize(newSize);
        makeCurrentContext();
    }

    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);

        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }

        // Dark blue background
        glClearColor(0.0f, 0.4f, .4f, 0.0f);

        // Enable depth test
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);

        // Accept fragment if it is closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // Cull triangles which normal is not towards the camera
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);

        setup();

        moveCursorToCenter();

        _tenk1.postaviTenk(_textureIDInShader, _mainPassProgramID, _matrixID);

        _tenk1.translirajTenk(glm::vec3{0.0f,0.0f,30.0f});

        _tenk2.postaviTenk(_textureIDInShader, _mainPassProgramID, _matrixID);

        _tenk2.translirajTenk(glm::vec3{9.0f,0.0f,-50.0f});

        //_tenk2.rotirajTenk(-3.14/2);

        //_tenk2.rotirajCijev(-3.14/2);


        _aktivniTenk = &_tenk1;

        _pasivniTenk = &_tenk2;

        _cameraProjectionMatrix = _perspectiveMatrix * _camera.getViewMatrix();

        _camera.setSkyBox(&_skyBox);
        _skyBox.setCamera(&_camera);

        _startTimePlayer = std::chrono::high_resolution_clock::now();

        setFocus();
    }

    bool prepareNextFrame() override
    {

         now = std::chrono::high_resolution_clock::now();

         char tmp[64];

         float health = _aktivniTenk->getHealth();
         //health
         gui::gl::Font& fnt = _text.getFont(2);
         fnt.reset();
         int len = td::format(tmp, "Health:%.1f %%", health);
         gui::gl::Font::Format format;
         format.scale = 1.0f;
         format.colorID = td::ColorID::DarkRed;
         format.hAlignment = td::HAlignment::Center;
         gui::gl::Point2d pt(950, 950);
         fnt.addString(pt, tmp, len, format);
         //crosshair
         len = td::format(tmp, "o");
         //                int len = SNPRINTF(tmp, 64, _TRUNCATE, "FPS:\n%.1f", _fps);
         format.scale = 0.5f;
         format.colorID = td::ColorID::GhostWhite;
         format.hAlignment = td::HAlignment::Center;
         pt.x = 965;
         pt.y = 237;
         fnt.addString(pt, tmp, len, format);
         //Player1/2
         if(!activePlayer)
            len = td::format(tmp, "Player 1");
         else
           len = td::format(tmp, "Player 2");
         //                int len = SNPRINTF(tmp, 64, _TRUNCATE, "FPS:\n%.1f", _fps);
         format.scale = 1.0f;
         format.colorID = td::ColorID::LightGoldenRodYellow;
         format.hAlignment = td::HAlignment::Left;
         pt.x = 20;
         pt.y = 960;
         gui::gl::Font& fnt3 = _text.getFont(0);
         fnt3.reset();
         fnt3.addString(pt, tmp, len, format);

        if (_pasivniTenk->getHealth() > 0)
        {
            std::chrono::duration<double> elapsedPlayer = now - _startTimePlayer;


            preostaloVrijeme = 20 - (int)elapsedPlayer.count();
            

            if (elapsedPlayer.count() > 20.0)
            {
                Tenk* pom = _aktivniTenk;

                _aktivniTenk = _pasivniTenk;

                _pasivniTenk = pom;
                _startTimePlayer = std::chrono::high_resolution_clock::now();
            }

            gui::gl::Font& fnt2 = _text.getFont(1);
            fnt2.reset();
            len = td::format(tmp, "Vrijeme: %ds", preostaloVrijeme);
            //                int len = SNPRINTF(tmp, 64, _TRUNCATE, "FPS:\n%.1f", _fps);
            format.scale = 0.5f;
            format.colorID = td::ColorID::GhostWhite;
            format.hAlignment = td::HAlignment::Left;
            pt.x = 20;
            pt.y = 10;
            fnt.addString(pt, tmp, len, format);

        }
        
        if(_aktivniTenk->getHealth() <= 0){
            Tenk* pom = _aktivniTenk;

            _aktivniTenk = _pasivniTenk;

            _pasivniTenk = pom;
            _startTimePlayer = std::chrono::high_resolution_clock::now();
        }


        gui::Point newCursorPos;
        bool inFocus = getCursorPosition(newCursorPos);
        moveCursorToCenter();
        gui::Point centerCursorPos;
        bool inFocus2 = getCursorPosition(centerCursorPos);

        auto delta = newCursorPos - centerCursorPos;

        if (inFocus && inFocus2)
        {
            // Yaw (lijevo-desno) - horizontalna rotacija cijevi
            rotacijaCijeviYaw += delta.x * 0.2f;
            _aktivniTenk->rotirajCijev(- rotacijaCijeviYaw * 0.0035f);

            // Pitch (gore-dole) - vertikalno podizanje/spustanje cijevi
            rotacijaCijeviPitch -= delta.y * 0.02f; // minus jer pomak gore treba povisiti cijev
            rotacijaCijeviPitch = std::clamp(rotacijaCijeviPitch, 0.0f, 30.0f); // ogranici elevaciju


            float podizanjeCijevi = 0;


            if (rotacijaCijeviPitch < 15)
                podizanjeCijevi = (15.0f - rotacijaCijeviPitch) * 1.5f * 3.14f / 180.0f;


            _aktivniTenk->podigniCijev(podizanjeCijevi); // proslijedi nagib
        }

        // Rotacija tenka
        if (rotirajTenkD) {
            _aktivniTenk->rotirajTenk(-0.01);
            if (isTankFootprintInForbiddenZone(_aktivniTenk->getTankFootprintCorners(), _forbiddenPoints))
                _aktivniTenk->rotirajTenk(0.01);
        }
        if (rotirajTenkL) {
            _aktivniTenk->rotirajTenk(0.01);
            if (isTankFootprintInForbiddenZone(_aktivniTenk->getTankFootprintCorners(), _forbiddenPoints))
                _aktivniTenk->rotirajTenk(-0.01);
        }

        // Translacija tenka
        if (pomjeriTenk1 || pomjeriTenk1R) {
            glm::vec3 moveVec = glm::vec3{ 0.0f, 0.0f, pomjeriTenk1 ? 0.05f : -0.05f };
            glm::vec3 staraPozicija = _aktivniTenk->position();

            _aktivniTenk->translirajTenk(moveVec);

            _udarioUPrepreku = isTankFootprintInForbiddenZone(_aktivniTenk->getTankFootprintCorners(), _forbiddenPoints);

           if (_udarioUPrepreku || _aktivniTenk->position()[0] > 93.0f || _aktivniTenk->position()[0] < -89.0f || _aktivniTenk->position()[2] > 54.0f || _aktivniTenk->position()[2] < -70.0f) {
               _aktivniTenk->translirajTenk(-moveVec); // poni�ti pomak
            }

            std::chrono::duration<double> elapsed = now - _startTimeThrottle;

            if (elapsed.count() > 4.5)
            {
                _zvukThrottle.play();
                _startTimeThrottle = std::chrono::high_resolution_clock::now();
            }

        }

        // Dobij poziciju tenka
        glm::vec3 pozicijaTenka = _aktivniTenk->position();

        // Izracunaj yaw/pitch u radijanima
        //float yawRad = (rotacijaTenka * 285.0f) * 0.0035f;  //ovo ako treba da se kamera okrece za tankBody ujesto za cijevi
        float yawRad = (-rotacijaCijeviYaw) * 0.0035f;
        float pitchRad = glm::radians(-rotacijaCijeviPitch);

        // Pozicija kamere iza cijevi, s obzirom na elevaciju
        glm::vec3 kameraOffset;
        kameraOffset.x = -orbitRadius * cos(pitchRad) * sin(yawRad);
        kameraOffset.y = -orbitRadius * sin(pitchRad) + 2.0f; // ni�e ako cijev gleda gore
        kameraOffset.z = -orbitRadius * cos(pitchRad) * cos(yawRad);

        glm::vec3 kameraPozicija = pozicijaTenka + kameraOffset;
        _camera.setPosition(kameraPozicija);

        // Kamera gleda tacno u smjer cijevi (naprijed u odnosu na yaw i pitch)
        glm::vec3 poglednaTacka;
        poglednaTacka.x = pozicijaTenka.x + 10.0f * cos(pitchRad) * sin(yawRad);
        poglednaTacka.y = pozicijaTenka.y + 10.0f * sin(pitchRad) + 1.5f;
        poglednaTacka.z = pozicijaTenka.z + 10.0f * cos(pitchRad) * cos(yawRad);

        _camera.lookAt(poglednaTacka);

        std::chrono::duration<double> elapsed = now - _startTimeMotor;

        if (elapsed.count() > 4.0)
        {
            _zvukMotora.play();
            _startTimeMotor = std::chrono::high_resolution_clock::now();
            animStep = 0;
        }

        if (checkCollision(_pasivniTenk->dajPozicijuShella(), *_aktivniTenk)) {
            _aktivniTenk->takeDamage(50);
            _pasivniTenk->pogodio();

            if (_pasivniTenk->getHealth() <= 0)
            {
                Tenk* pom = _aktivniTenk;

                _aktivniTenk = _pasivniTenk;

                _pasivniTenk = pom;
                _startTimePlayer = std::chrono::high_resolution_clock::now();
            }

        }
        
        return true;
    }




    void onDraw(const gui::Rect& rect) override
    {

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Update projection * view
        glm::mat4 ProjectionMatrix = _camera.MojgetPerspectiveMatrix();
        glm::mat4 ViewMatrix = _camera.MojgetViewMatrix();
        _cameraProjectionMatrix = ProjectionMatrix * ViewMatrix;

        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });

        _skyBox.render();
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);

        reSetup();

        // Use main scene shader
        glUseProgram(_mainPassProgramID);

        //---------------------------------crtanje grada-------------------------------------------------------
        {
            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { 0.0f + naprijed,0.0f,0.0f });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _textureMain);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
            glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // 2nd attribute buffer : UVs
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
            glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // 3rd attribute buffer : normals
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbuffer);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCnt,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);


        }

        _aktivniTenk->crtajTenk(_textureIDInShader, _mainPassProgramID, _matrixID, _camera);

        _pasivniTenk->crtajTenk(_textureIDInShader, _mainPassProgramID, _matrixID, _camera);


        glUseProgram(0);
        glDisable(GL_DEPTH_TEST);

        GLint texBefore, texAfter;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &texBefore);
        _text.render();
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &texAfter);

        glUseProgram(_mainPassProgramID);
        glEnable(GL_DEPTH_TEST);
    }

    void reSetup() {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        glGenBuffers(1, &_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    }

    bool onScroll(const gui::InputDevice& inputDevice) override
    {
        if (_cameraControl)
            _camera.scroll(inputDevice.getFramePoint().y);

        return true;
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        char keyPress = std::tolower(key.getChar());

        if (keyPress == ' ')
            //_zvukFire.play();

        if (keyPress == 'i')
            pomjeriTenk1 = true;
        if (keyPress == 'k')
            pomjeriTenk1R = true;
        if (keyPress == 'l')
            rotirajTenkD = true;
        if (keyPress == 'j')
            rotirajTenkL = true;

        if (key.isAltCtrlOrCmdPressed() && (key.getChar() == 'q' || key.getChar() == 'Q'))
            this->getParentWindow()->close();

        if (_cameraControl)
        {
            if (keyPress == 'w') {
            pomjeriTenk1 = true;
            }
            if (keyPress == 's') {

                pomjeriTenk1R = true;
            }
            if (keyPress == 'a') {
                rotirajTenkL = true;
            }
            if (keyPress == 'd') {
                rotirajTenkD = true;
            }

        }

        return true;
    }


    bool onKeyReleased(const gui::Key& key) override
    {
        char keyPress = std::tolower(key.getChar());

        if (keyPress == ' ')
            _cameraControl = !_cameraControl;

        if (keyPress == 'i') 
            pomjeriTenk1 = false;
        if (keyPress == 'k')
            pomjeriTenk1R = false;
        if (keyPress == 'l') 
            rotirajTenkD = false;
        if (keyPress == 'j')
            rotirajTenkL = false;

        if (_cameraControl)
        {
            if (keyPress == 'w') {
                pomjeriTenk1 = false;
                _zvukThrottle.stop();

                 _startTimeThrottle = std::chrono::high_resolution_clock::time_point{};
            }
            if (keyPress == 's') {
                
                pomjeriTenk1R = false;

                _zvukThrottle.stop();

                _startTimeThrottle = std::chrono::high_resolution_clock::time_point{};
            }
            if (keyPress == 'a') {
                rotirajTenkL = false;
            }
            if (keyPress == 'd') {
                rotirajTenkD = false;
            }


        }

        return true;
    }

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override{

       if(_aktivniTenk->pucaj())
          _zvukFire.play();

       if (_pasivniTenk->getHealth() > 0)
       {
           Tenk* pom = _aktivniTenk;

           _aktivniTenk = _pasivniTenk;

           _pasivniTenk = pom;
       }

       _startTimePlayer = std::chrono::high_resolution_clock::now();

       activePlayer = !activePlayer;

    }

/*
    bool isInForbiddenZone(glm::vec3 pos, const std::vector<glm::vec3>& forbiddenPoints, float tolerance = 1.0f) {
        for (const auto& p : forbiddenPoints) {
            if (glm::distance(glm::vec2(pos.x, pos.z), glm::vec2(p.x, p.z)) < tolerance) {
                return true; // preblizu zgradi/trotoaru
            }
        }
        return false;
    }*/

    bool checkCollision(const glm::vec3 shellPosition, const Tenk& tank) {
        AABB box = tank.getAABB();

        float shellRadius = 0.5f;

        float closestX = std::max(box.min.x, std::min(shellPosition.x, box.max.x));
        float closestY = std::max(box.min.y, std::min(shellPosition.y, box.max.y));
        float closestZ = std::max(box.min.z, std::min(shellPosition.z, box.max.z));

        float distanceX = shellPosition.x - closestX;
        float distanceY = shellPosition.y - closestY;
        float distanceZ = shellPosition.z - closestZ;

        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ);

        return distanceSquared < (shellRadius * shellRadius);
    }

/*
    bool isAABBInForbiddenZoneFast(const AABB& aabb, const std::vector<glm::vec3>& forbiddenPoints, float tolerance = 0.3f) {
        std::vector<glm::vec2> testPoints = {
            { aabb.min.x, aabb.min.z },
            { aabb.max.x, aabb.min.z },
            { aabb.min.x, aabb.max.z },
            { aabb.max.x, aabb.max.z },
            { (aabb.min.x + aabb.max.x) * 0.5f, (aabb.min.z + aabb.max.z) * 0.5f }  // center point
        };

        for (const auto& tp : testPoints) {
            for (const auto& p : forbiddenPoints) {
                if (glm::distance(tp, glm::vec2(p.x, p.z)) < tolerance) {
                    return true;
                }
            }
        }

        return false;
    }*/


    bool isTankFootprintInForbiddenZone(const std::vector<glm::vec3>& tankCorners, const std::vector<glm::vec3>& forbiddenPoints, float tolerance = 0.35f) {
        for (const auto& corner : tankCorners) {
            for (const auto& forbidden : forbiddenPoints) {
                if (glm::distance(glm::vec2(corner.x, corner.z), glm::vec2(forbidden.x, forbidden.z)) < tolerance) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    MainView()
        : _camera({ (float)1 , (float)1 }, glm::vec3(-2, 3, -20), glm::vec3(0, 1, 0), 0.1, 1000, 0.003, 0.05, 0.05, 35, 100, -45, true, true)
        , _textHouse(":cityTex")
        , _zvukMotora(":tankEngine")
        , _zvukFire(":tankFire")
        , _zvukThrottle(":tankThrottle")
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        reqContext.setPreferredFrameRateRange(60, 60);
        createContext(reqContext);
        showCursor(false);
        setCamera(&_camera);
    }

    ~MainView()
    {
        makeCurrentContext();
        cleanResources();
    }

};
