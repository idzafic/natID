#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "Program.h"
#include <gui/gl/CommandList.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>
#include <gui/Color.h>
#include <gui/gl/Triang2D.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rnd/Perlin.h>
#include <cnt/StringBuilder.h>
#include <gui/gl/Textures.h> 
#include "Camera.h"
#include "Stopwatch.h" 
#include "DialogFrequency.h"

#define numVerticesToReserve  270000  // (512+1)*(512+1) = ~263k
#define numIndicesToReserve  1600000 // 512*512*6 = ~1.57M

// Main OpenGL view class for procedural terrain generation 
class ViewTerrainGen : public gui::gl::View
{
    // Camera and timing for smooth control and frame update
    Camera _camera;
    Stopwatch _clock;
    bool _cameraControl = true;

    // Matrices for rendering pipeline and lighting setup
    glm::mat4 _mvpMatrix;
    glm::mat4 _modelViewMat;
    glm::mat4 _normalMat;
    glm::mat4 _modelMatrix;

    // GPU buffer 
    gui::gl::Buffer _gpuBuffer;

    // Sun direction and color for lighting
    glm::vec3 _sunDirection = glm::normalize(glm::vec3(0.5f, -1.0f, -0.5f));
    glm::vec3 _sunColor = glm::vec3(1.f);

    Program _mainPass;
    gui::Size _size;

    // Framebuffer and depth texture for shadow mapping
    GLuint _shadowMapFBO = 0;
    GLuint _depthTextureID = 0;

    // Terrain parameters
    int _numOfLines;
    int _gridSize;
    float _terrainHalfWidth;
    float _cameraHeightOffset = 1.5f;

    double _perlinFrequency = 7.0; // Terrain generation frequency

    const td::UINT4 _cFrqDlgID = 101; // Unique ID for the frequency dialog

    // Terrain textures
    gui::gl::Textures _textures;
  //  gui::Texture _texWater;
    gui::Texture _texSand;
    gui::Texture _texLightGrass;
    gui::Texture _texGrass;
    gui::Texture _texDarkGrass;
    gui::Texture _texStonyGrass;
    gui::Texture _texRock;
    gui::Texture _texSnow;

    // Height map storage
    std::vector<std::vector<float>> _heightMap;

    // Fog parameters
    glm::vec3 _fogColor;
    float _fogStartDistance;
    float _fogEndDistance;

private:

    // Calculate terrain normal vector for lighting based on height differences
    glm::vec3 calculateNormal(int x, int z, int size, const std::vector<std::vector<float>>& heightMap) {
        float heightL = heightMap[z][x > 0 ? x - 1 : x];
        float heightR = heightMap[z][x < size - 1 ? x + 1 : x];
        float heightD = heightMap[z > 0 ? z - 1 : z][x];
        float heightU = heightMap[z < size - 1 ? z + 1 : z][x];

        glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
        return glm::normalize(normal);
    }

    // Create OpenGL framebuffer and depth texture for shadow mapping
    void createDepthFBO()
    {
        if (_size.width == 0 || _size.height == 0)
            return;

        if (_depthTextureID != 0)
        {
            glDeleteTextures(1, &_depthTextureID);
            _depthTextureID = 0;
        }

        if (_shadowMapFBO != 0)
        {
            glDeleteFramebuffers(1, &_shadowMapFBO);
            _shadowMapFBO = 0;
        }

        glGenFramebuffers(1, &_shadowMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO);

        glGenTextures(1, &_depthTextureID);
        glBindTexture(GL_TEXTURE_2D, _depthTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _size.width, _size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTextureID, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Compile and link GLSL shaders for terrain rendering
    bool setupShaders()
    {
        if (!_mainPass.link(":shVert", ":shFrag"))
            return false;

        return true;
    }

    // Generate terrain vertex and index data using Perlin noise
    void perlinGenAndSetup(std::vector<float>& vertices, std::vector<td::UINT4>& indices, double frequency) {

        // Parameters for Perlin noise generation
        int octaves = 5;
        uint32_t seed = 44;

        const rnd::Perlin perlin(seed);

        // Setup grid size and terrain dimensions
        _numOfLines = 512;
        _gridSize = _numOfLines + 1;
        _terrainHalfWidth = (_numOfLines) / 2.0f;

        // Scale factors for noise sampling
        const double fx = _gridSize / frequency;
        const double fy = _gridSize / frequency;

        // Constants controlling terrain height and texture UV tiling
        const float heightMultiplier = 35.0f;
        const float peakFactor = 1.8f;
        const float uvTilingFactor = 36.0f;

        // Generate height map using Perlin noise with multiple octaves
        _heightMap.assign(_gridSize, std::vector<float>(_gridSize));
        for (int z = 0; z < _gridSize; ++z) {
            for (int x = 0; x < _gridSize; ++x) {
                float rawHeight = perlin.accumulatedOctaveNoise2D_0_1(x / fx, z / fy, octaves);
                float calculatedHeight = pow(rawHeight, peakFactor) * heightMultiplier;
               float finalHeight = std::max(calculatedHeight, 2.0f); // Ensure minimum height for water level
               // float finalHeight = calculatedHeight;
                _heightMap[z][x] = finalHeight;
            }
        }

        // Reserve space for vertex attributes: position(3), normal(3), UV(2)
        vertices.resize(_gridSize * _gridSize * 8);
        int index = 0;

        // Fill vertex buffer with positions, normals, and texture coordinates
        for (int z = 0; z < _gridSize; ++z) {
            for (int x = 0; x < _gridSize; ++x) {
                vertices[index++] = (float)x - _terrainHalfWidth;       // X position centered
                vertices[index++] = _heightMap[z][x];                   // Y (height)
                vertices[index++] = (float)z - _terrainHalfWidth;       // Z position centered

                glm::vec3 normal = calculateNormal(x, z, _gridSize, _heightMap); // Calculate normal for lighting
                vertices[index++] = normal.x;
                vertices[index++] = normal.y;
                vertices[index++] = normal.z;

                // UV coordinates scaled for texture tiling
                vertices[index++] = ((float)x / _numOfLines) * uvTilingFactor;
                vertices[index++] = ((float)z / _numOfLines) * uvTilingFactor;
            }
        }

        // Reserve space for triangle indices (2 triangles per quad)
        indices.resize(_numOfLines * _numOfLines * 6);
        int index1 = 0;

        // Generate indices for triangle strips forming the terrain grid
        for (int z = 0; z < _numOfLines; ++z) {
            for (int x = 0; x < _numOfLines; ++x) {
                int topLeft = z * _gridSize + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * _gridSize + x;
                int bottomRight = bottomLeft + 1;

                // First triangle of the quad
                indices[index1++] = topLeft;
                indices[index1++] = bottomLeft;
                indices[index1++] = topRight;

                // Second triangle of the quad
                indices[index1++] = topRight;
                indices[index1++] = bottomLeft;
                indices[index1++] = bottomRight;
            }
        }
    }

    void regenerateTerrain()
    {
        //Reci OpenGL-u da je ovaj prozor ponovno aktivan.
        makeCurrentContext();

        std::vector<float> vertices;
        std::vector<td::UINT4> indices;

        perlinGenAndSetup(vertices, indices, _perlinFrequency);

        _gpuBuffer.reset();
        _gpuBuffer.init(3, numVerticesToReserve, numIndicesToReserve, { gui::gl::DataType::vec3, gui::gl::DataType::vec3, gui::gl::DataType::vec2 });

        _gpuBuffer.appendVertices(vertices.data(), vertices.size() / 8);
        _gpuBuffer.appendIndices(indices.data(), indices.size());

        auto pPyCmd = _gpuBuffer.createCommand();
        pPyCmd->createDrawElements(gui::gl::Primitive::Triangles, 0, indices.size());
        _gpuBuffer.encode(pPyCmd);

        if (!_gpuBuffer.commit()) {
            mu::dbgLog("ERROR! Cannot commit buffer after regeneration");
            return;
        }

        _mainPass.setBuffer(&_gpuBuffer);
        reDraw();
    }

    // Retrieve terrain height at a given world coordinate by bilinear interpolation
    float getTerrainHeight(float worldX, float worldZ) const
    {
        if (_heightMap.empty()) return 0.0f;

        float mapX = worldX + _terrainHalfWidth;
        float mapZ = worldZ + _terrainHalfWidth;

        if (mapX < 0 || mapX >= _gridSize - 1 || mapZ < 0 || mapZ >= _gridSize - 1) {
            return 0.0f;
        }

        int gridX = static_cast<int>(mapX);
        int gridZ = static_cast<int>(mapZ);

        float xCoord = mapX - gridX;
        float zCoord = mapZ - gridZ;

        float h00 = _heightMap[gridZ][gridX];
        float h10 = _heightMap[gridZ][gridX + 1];
        float h01 = _heightMap[gridZ + 1][gridX];
        float h11 = _heightMap[gridZ + 1][gridX + 1];

        float bottomHeight = (1 - xCoord) * h00 + xCoord * h10;
        float topHeight = (1 - xCoord) * h01 + xCoord * h11;
        float finalHeight = (1 - zCoord) * bottomHeight + zCoord * topHeight;

        return finalHeight;
    }

    // Initialize GPU buffers, generate terrain data, upload to GPU, load textures, and set OpenGL state
    void setup()
    {
        // Koristimo '3' jer je tako bilo u originalnom kodu koji je radio.
        _gpuBuffer.init(3, numVerticesToReserve, numIndicesToReserve, { gui::gl::DataType::vec3, gui::gl::DataType::vec3, gui::gl::DataType::vec2 });

        std::vector<float> vertices;
        std::vector<td::UINT4> indices;

        // Generiraj i postavi početni teren
        perlinGenAndSetup(vertices, indices, _perlinFrequency);

        td::UINT4 nVertices = vertices.size() / 8;
        _gpuBuffer.appendVertices(vertices.data(), nVertices);
        td::UINT4 nIndices = indices.size();
        _gpuBuffer.appendIndices(indices.data(), nIndices);

        auto pPyCmd = _gpuBuffer.createCommand();
        pPyCmd->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);
        _gpuBuffer.encode(pPyCmd);

        // Pošalji podatke na grafičku karticu
        if (!_gpuBuffer.commit()) {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU during initial setup");
            return;
        }

        _mainPass.setBuffer(&_gpuBuffer);
        _textures.append({ &_texSand, &_texLightGrass, &_texGrass, &_texDarkGrass, &_texStonyGrass, &_texRock, &_texSnow });
        _mainPass.setTextures(&_textures);

        createDepthFBO();
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
    }



protected:

    // Resize handler to update viewport and context
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        _camera.setViewportSize(newSize);
        makeCurrentContext();
    }

    // Initialize OpenGL context, shaders and setup buffers
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

        setup();

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("OpenGL error: %x", error);
            assert(false);
        }
    }

    // Update camera and matrices before each frame render
    bool prepareNextFrame() override
    {
        // Update camera rotation from mouse input
        if (_cameraControl)
        {
            gui::Point newCursorPos;
            bool inFocus = getCursorPosition(newCursorPos);
            moveCursorToCenter();
            gui::Point centerCursorPos;
            bool inFocus2 = getCursorPosition(centerCursorPos);

            if (inFocus && inFocus2)
            {
                auto delta = newCursorPos - centerCursorPos;
                _camera.yawPitchMouse(delta);
            }
        }

        // Process movement input and apply terrain collision
        _camera.processInputs(_clock.elapsed(), _cameraControl);

        glm::vec3 camPos = _camera.position();
        float terrainHeight = getTerrainHeight(camPos.x, camPos.z);
        float minAllowedY = terrainHeight + _cameraHeightOffset;

        if (camPos.y < minAllowedY)
        {
            camPos.y = minAllowedY;
            _camera.setPosition(camPos);
        }

        // Compute final matrices for rendering
        glm::mat4 projectionMatrix = _camera.getPerspectiveMatrix();
        glm::mat4 viewMatrix = _camera.getViewMatrix();

        _modelMatrix = glm::mat4(1.0f);
        _modelViewMat = viewMatrix * _modelMatrix;
        _normalMat = glm::transpose(glm::inverse(_modelViewMat));
        _mvpMatrix = projectionMatrix * viewMatrix * _modelMatrix;

        return true;
    }

    // Render the terrain scene each frame
    void onDraw(const gui::Rect& rect) override
    {
        activateScreenFB();
        gui::gl::Context::clear(td::ColorID::LightBlue);
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });

        _mainPass.activate();

        _mainPass.setTime(_clock.elapsed());
        _mainPass.setCameraPos(_camera.position());

        _mainPass.setMVP(_mvpMatrix);
        _mainPass.setMV(_modelViewMat);
        _mainPass.setN(_normalMat);
        _mainPass.setLight(_sunDirection, _sunColor);
        _mainPass.setFog(_fogColor, _fogStartDistance, _fogEndDistance);

        // ISPRAVAK: Koristimo ispravne indekse (0-6)
        _mainPass.setTextureSand(0);          // Sand je sada na poziciji 0
        _mainPass.setTextureLightGrass(1);    // LightGrass je na poziciji 1
        _mainPass.setTextureGrass(2);         // ...itd.
        _mainPass.setTextureDarkGrass(3);
        _mainPass.setTextureStonyGrass(4);
        _mainPass.setTextureRock(5);
        _mainPass.setTextureSnow(6);          // Snow je na zadnjoj poziciji, 6

        _mainPass.execute();
        _mainPass.deActivate();
    }



    // Keyboard input handler for camera and object control
    bool onKeyPressed(const gui::Key& key) override
    {
        auto pressedKey = td::toLower(key.getChar());

        if (pressedKey == ' ')
            _cameraControl = !_cameraControl;

        if (_cameraControl)
        {
            if (pressedKey == 'w') _camera.move(Camera::Movement::FORWARD);
            if (pressedKey == 's') _camera.move(Camera::Movement::BACKWARD);
            if (pressedKey == 'a') _camera.move(Camera::Movement::LEFT);
            if (pressedKey == 'd') _camera.move(Camera::Movement::RIGHT);

            if (pressedKey == 'q') _camera.yawKB(Camera::Yaw::DEC);
            if (pressedKey == 'e') _camera.yawKB(Camera::Yaw::INC);
        }

        return true;
    }

    // Keyboard release handler to stop camera movement
    bool onKeyReleased(const gui::Key& key) override
    {
        auto releasedKey = td::toLower(key.getChar());
        if (_cameraControl)
        {
            if (releasedKey == 'w') _camera.unMove(Camera::Movement::FORWARD);
            if (releasedKey == 's') _camera.unMove(Camera::Movement::BACKWARD);
            if (releasedKey == 'a') _camera.unMove(Camera::Movement::LEFT);
            if (releasedKey == 'd') _camera.unMove(Camera::Movement::RIGHT);
        }
        return true;
    }

    // Mouse scroll handler for camera zoom control
    bool onScroll(const gui::InputDevice& inputDevice) override
    {
        if (_cameraControl)
            _camera.scroll(inputDevice.getFramePoint().y);
        return true;
    }

    // Handles actions from toolbar buttons (forwarded from MainWindow)
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        // Deconstruct IDs from the descriptor
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();

        if (actionID == 10) // Matches frequency button ID from ToolBar.h
        {
            // Check if dialog is already open
            auto pDlg = getAttachedWindow(_cFrqDlgID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                // Create and open the frequency dialog
                DialogFrequency* pFrequencyDlg = new DialogFrequency(this, _cFrqDlgID);
                pFrequencyDlg->keepOnTopOfParent();
                pFrequencyDlg->open();
            }
            return true;
        }
        return false;
    }

   
    // Handles the result from the closed dialog
    bool onClick(gui::Dialog* dlg, td::UINT4 dlgID) override
    {
        if (dlgID == _cFrqDlgID)
        {
            if (dlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
            {
                DialogFrequency* pDlg = static_cast<DialogFrequency*>(dlg);
                td::String freqStr = pDlg->getFrequency();
                double newFreq = freqStr.toDouble();

                if (newFreq > 0 && newFreq<25)
                {
                    _perlinFrequency = newFreq;
                    regenerateTerrain();
                }
                else
                {
                    gui::Alert::show(tr("Error"), tr("Invalid frequency. Please enter a valid number."));
                }
            }
            return true;
        }
        return false;
    }

public:
    ViewTerrainGen()
        : _camera({ (float)1, (float)1 }, glm::vec3(0, 10, 30), glm::vec3(0, 1, 0), 0.1f, 1000.0f, 0.003, 0.1, 0.1, 60.0f, -90.0f, -20.0f, true, true)
        /*, _texWater(":water")*/
        , _texSand(":sand")
        , _texLightGrass(":lightGreen")
        , _texGrass(":grass")
        , _texDarkGrass(":darkGrass")
        , _texStonyGrass(":stonyGrass")
        , _texRock(":mountain")
        , _texSnow(":snow")
        , _gridSize(0)
        , _terrainHalfWidth(0.0f)
        , _fogStartDistance(100.0f)
        , _fogEndDistance(200.0f)
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3, gui::gl::StencilBuffer::Size::B1);
        createContext(reqContext, { gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::Zoom });

        float r, g, b, a;
        gui::Color::getRGBA(td::ColorID::LightBlue, r, g, b, a);
        _fogColor = glm::vec3(r, g, b);

        showCursor(false);
        setFocus();
    }

    ~ViewTerrainGen()
    {
        makeCurrentContext();
        if (_depthTextureID != 0) glDeleteTextures(1, &_depthTextureID);
        if (_shadowMapFBO != 0) glDeleteFramebuffers(1, &_shadowMapFBO);
    }

};