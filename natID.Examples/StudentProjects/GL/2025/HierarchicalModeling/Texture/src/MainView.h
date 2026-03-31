#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include "ShapeGenerator.h"
#include "Camera.h"
#include "objloader.h" 

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <cmath>

// Struktura koja cuva sve OpenGL podatke za jedan vidljivi dio modela
struct ModelComponent
{
    GLuint vao = 0;
    GLuint vbo_vertices = 0;
    GLuint vbo_normals = 0;
    GLuint vbo_uvs = 0;
    GLuint ebo = 0;
    td::UINT4 indicesCount = 0;
    GLuint textureID = 0;
};

// cvor u scenoskom grafu, definira hijerarhiju i transformacije
struct TreeNode {
    glm::mat4 localTransform; // Transformacija u odnosu na roditelja
    glm::mat4 baseTransform;  // Pocetna, "mirna" transformacija
    ModelComponent* model = nullptr;
    // Pokazivaci za LCRS (Left-Child, Right-Sibling) stablo
    TreeNode* parent = nullptr;
    TreeNode* child = nullptr;
    TreeNode* sibling = nullptr;
    std::vector<TreeNode*> children; // Pomocni vektor za lakse upravljanje memorijom

    ~TreeNode() {
        for (auto c : children) {
            delete c;
        }
    }

    void addChild(TreeNode* newChild) {
        newChild->parent = this;
        if (child == nullptr) {
            child = newChild;
        }
        else {
            TreeNode* lastSibling = child;
            while (lastSibling->sibling != nullptr) {
                lastSibling = lastSibling->sibling;
            }
            lastSibling->sibling = newChild;
        }
        children.push_back(newChild);
    }
};

class MainView : public gui::gl::View
{
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    td::UINT4 _mainPassProgramID;

    // Uniform lokacije za shadere
    GLuint _textureIDInShader;
    GLuint _matrixID;

    Camera _kamera;

    // Geometrija za dijelove humanoida
    ModelComponent _trup, _glava, _lijevaNogaGore, _lijevaNogaDole, _desnaNogaGore, _desnaNogaDole,
        _lijevaRukaGore, _lijevaRukaDole, _desnaRukaGore, _desnaRukaDole;
    ModelComponent _lijevoRame, _desnoRame, _lijeviLakat, _desniLakat,
        _lijeviKuk, _desniKuk, _lijevoKoljeno, _desnoKoljeno;

    // Teksture
    gui::Texture _shirtTex;
    gui::Texture _skinTex;
    gui::Texture _jeansTex;

    // Scenski graf
    TreeNode* _humanoidRoot = nullptr; // Korijen cijelog lika
    std::map<std::string, TreeNode*> _nodeMap; // Mapa za laksi pristup cvorovima po imenu

    // Varijable za animaciju i kretanje
    float _ugaoHoda = 0.0f;
    bool _kretanjeNaprijed = false, _kretanjeNazad = false, _rotacijaLijevo = false, _rotacijaDesno = false;
    float _humanoidPozicijaX = 0.0f, _humanoidPozicijaZ = 0.0f;
    float _humanoidUgaoRotacije = glm::pi<float>();
    const float _baseBrzinaHoda = 4.0f;
    const float _baseBrzinaKretanja = 5.0f;
    const float _sprintFaktor = 2.0f;
    bool _sprintAktivan = false;
    bool _mahanjeAktivno = false;
    float _ugaoMahanja = 0.0f;
    float _progressPodizanjaRuke = 0.0f; // Za glatku tranziciju mahanja

    // Varijable za kameru
    bool _kameraZoomIn = false;
    bool _kameraZoomOut = false;
    float _kameraUdaljenost = 15.0f;
    const float _kameraBrzinaZuma = 15.0f;
    float _kameraUgaoOkoLika = 0.0f;

    std::chrono::steady_clock::time_point _lastFrameTime;

private:
    // Povezuje vertex i fragment shader u jedan program
    bool setupShaders()
    {
        _mainPassProgramID = linkProgram(":mainV", ":mainF");
        assert(_mainPassProgramID > 0);
        return true;
    }

    // Kreira OpenGL buffere na osnovu podataka iz ShapeGeneratora
    void createAndLoadModel(const ShapeData& shapeData, GLuint textureID, ModelComponent& component)
    {
        component.textureID = textureID;
        component.indicesCount = (td::UINT4)shapeData.indices.size();

        glGenVertexArrays(1, &component.vao);
        glBindVertexArray(component.vao);

        glGenBuffers(1, &component.vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, shapeData.vertices.size() * sizeof(glm::vec3), shapeData.vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &component.vbo_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_uvs);
        glBufferData(GL_ARRAY_BUFFER, shapeData.uvs.size() * sizeof(glm::vec2), shapeData.uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &component.vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, shapeData.normals.size() * sizeof(glm::vec3), shapeData.normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &component.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapeData.indices.size() * sizeof(unsigned int), shapeData.indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    // Iscrtava jednu komponentu koristeci njene podatke i proslijeđenu matricu
    void drawComponent(const ModelComponent& component, const glm::mat4& modelMatrix)
    {
        glm::mat4 MVP = _perspectiveMatrix * _viewMatrix * modelMatrix;
        glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(component.vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, component.textureID);
        glUniform1i(_textureIDInShader, 0);

        glDrawElements(GL_TRIANGLES, component.indicesCount, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);
    }

    // Kreira hijerarhijsko stablo (scenski graf) i povezuje dijelove tijela
    void setupSceneGraph() {
        delete _humanoidRoot;
        _nodeMap.clear();

        // Glavni korijen u ishodistu svijeta
        _humanoidRoot = new TreeNode();
        _humanoidRoot->baseTransform = glm::mat4(1.0f);
        _nodeMap["root"] = _humanoidRoot;

        // Trup je dijete glavnog korijena
        auto* torsoNode = new TreeNode();
        torsoNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        torsoNode->model = &_trup;
        _humanoidRoot->addChild(torsoNode);
        _nodeMap["torso"] = torsoNode;

        // Dimenzije dijelova tijela za pozicioniranje
        float duzinaNadlaktice = 0.7f;
        float duzinaPodlaktice = 0.7f;
        float duzinaNadkoljenice = 0.9f;
        float duzinaPotkoljenice = 0.8f;

        // GLAVA
        auto* headNode = new TreeNode();
        headNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.1f, 0.0f));
        headNode->model = &_glava;
        torsoNode->addChild(headNode);
        _nodeMap["head"] = headNode;

        // LIJEVA RUKA
        auto* leftShoulderNode = new TreeNode();
        leftShoulderNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.7f, 0.0f));
        leftShoulderNode->model = &_lijevoRame;
        torsoNode->addChild(leftShoulderNode);
        _nodeMap["left_shoulder"] = leftShoulderNode;

        auto* leftUpperArmNode = new TreeNode();
        leftUpperArmNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, -duzinaNadlaktice / 2.0f, 0.0f));
        leftUpperArmNode->model = &_lijevaRukaGore;
        leftShoulderNode->addChild(leftUpperArmNode);
        _nodeMap["left_upper_arm"] = leftUpperArmNode;

        auto* leftElbowNode = new TreeNode();
        leftElbowNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaNadlaktice / 2.0f, 0.0f));
        leftElbowNode->model = &_lijeviLakat;
        leftUpperArmNode->addChild(leftElbowNode);
        _nodeMap["left_elbow"] = leftElbowNode;

        auto* leftLowerArmNode = new TreeNode();
        leftLowerArmNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaPodlaktice / 2.0f, 0.0f));
        leftLowerArmNode->model = &_lijevaRukaDole;
        leftElbowNode->addChild(leftLowerArmNode);
        _nodeMap["left_lower_arm"] = leftLowerArmNode;

        // DESNA RUKA
        auto* rightShoulderNode = new TreeNode();
        rightShoulderNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.6f, 0.7f, 0.0f));
        rightShoulderNode->model = &_desnoRame;
        torsoNode->addChild(rightShoulderNode);
        _nodeMap["right_shoulder"] = rightShoulderNode;

        auto* rightUpperArmNode = new TreeNode();
        rightUpperArmNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.05f, -duzinaNadlaktice / 2.0f, 0.0f));
        rightUpperArmNode->model = &_desnaRukaGore;
        rightShoulderNode->addChild(rightUpperArmNode);
        _nodeMap["right_upper_arm"] = rightUpperArmNode;

        auto* rightElbowNode = new TreeNode();
        rightElbowNode->baseTransform = glm::translate(rightUpperArmNode->baseTransform, glm::vec3(0.0f, -duzinaNadlaktice / 2.0f, 0.0f));
        rightElbowNode->model = &_desniLakat;
        rightShoulderNode->addChild(rightElbowNode);
        _nodeMap["right_elbow"] = rightElbowNode;

        auto* rightLowerArmNode = new TreeNode();
        rightLowerArmNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaPodlaktice / 2.0f, 0.0f));
        rightLowerArmNode->model = &_desnaRukaDole;
        rightElbowNode->addChild(rightLowerArmNode);
        _nodeMap["right_lower_arm"] = rightLowerArmNode;

        // LIJEVA NOGA
        auto* leftHipNode = new TreeNode();
        leftHipNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35f, -0.85f, 0.0f));
        leftHipNode->model = &_lijeviKuk;
        torsoNode->addChild(leftHipNode);
        _nodeMap["left_hip"] = leftHipNode;

        auto* leftUpperLegNode = new TreeNode();
        leftUpperLegNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        leftUpperLegNode->model = &_lijevaNogaGore;
        leftHipNode->addChild(leftUpperLegNode);
        _nodeMap["left_upper_leg"] = leftUpperLegNode;

        auto* leftKneeNode = new TreeNode();
        leftKneeNode->baseTransform = glm::translate(leftUpperLegNode->baseTransform, glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        leftKneeNode->model = &_lijevoKoljeno;
        leftHipNode->addChild(leftKneeNode);
        _nodeMap["left_knee"] = leftKneeNode;

        auto* leftLowerLegNode = new TreeNode();
        leftLowerLegNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaPotkoljenice / 2.0f, 0.0f));
        leftLowerLegNode->model = &_lijevaNogaDole;
        leftKneeNode->addChild(leftLowerLegNode);
        _nodeMap["left_lower_leg"] = leftLowerLegNode;

        // DESNA NOGA
        auto* rightHipNode = new TreeNode();
        rightHipNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f, -0.85f, 0.0f));
        rightHipNode->model = &_desniKuk;
        torsoNode->addChild(rightHipNode);
        _nodeMap["right_hip"] = rightHipNode;

        auto* rightUpperLegNode = new TreeNode();
        rightUpperLegNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        rightUpperLegNode->model = &_desnaNogaGore;
        rightHipNode->addChild(rightUpperLegNode);
        _nodeMap["right_upper_leg"] = rightUpperLegNode;

        auto* rightKneeNode = new TreeNode();
        rightKneeNode->baseTransform = glm::translate(rightUpperLegNode->baseTransform, glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        rightKneeNode->model = &_desnoKoljeno;
        rightHipNode->addChild(rightKneeNode);
        _nodeMap["right_knee"] = rightKneeNode;

        auto* rightLowerLegNode = new TreeNode();
        rightLowerLegNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -duzinaPotkoljenice / 2.0f, 0.0f));
        rightLowerLegNode->model = &_desnaNogaDole;
        rightKneeNode->addChild(rightLowerLegNode);
        _nodeMap["right_lower_leg"] = rightLowerLegNode;

        // Postavi pocetne transformacije za sve cvorove
        for (auto const& [key, val] : _nodeMap) {
            if (val) val->localTransform = val->baseTransform;
        }
    }

    // Rekurzivno iscrtavanje stabla
    void traverseAndDraw(TreeNode* node, const glm::mat4& parentModelMatrix) {
        if (!node) return;

        // Izracunaj finalnu ("svjetsku") matricu za ovaj cvor
        glm::mat4 currentModelMatrix = parentModelMatrix * node->localTransform;

        if (node->model) {
            // Skaliranje specificno za svaki dio tijela
            float duzinaNadlaktice = 0.7f;
            float duzinaPodlaktice = 0.7f;
            float duzinaNadkoljenice = 0.9f;
            float duzinaPotkoljenice = 0.8f;

            glm::mat4 finalDrawMatrix = currentModelMatrix;
            if (node->model == &_trup)             finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(1.0f, 1.7f, 0.75f));
            else if (node->model == &_glava)       finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.5f));
            else if (node->model == &_lijevoRame)  finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.18f));
            else if (node->model == &_lijevaRukaGore) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.2f, duzinaNadlaktice, 0.25f));
            else if (node->model == &_lijevaRukaDole) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.18f, duzinaPodlaktice, 0.22f));
            else if (node->model == &_lijeviLakat) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.12f));
            else if (node->model == &_desnoRame)  finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.18f));
            else if (node->model == &_desnaRukaGore) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.2f, duzinaNadlaktice, 0.25f));
            else if (node->model == &_desnaRukaDole) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.18f, duzinaPodlaktice, 0.22f));
            else if (node->model == &_desniLakat) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.12f));
            else if (node->model == &_lijeviKuk) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.20f));
            else if (node->model == &_lijevaNogaGore) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.3f, duzinaNadkoljenice, 0.35f));
            else if (node->model == &_lijevoKoljeno) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.16f));
            else if (node->model == &_lijevaNogaDole) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.25f, duzinaPotkoljenice, 0.3f));
            else if (node->model == &_desniKuk) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.20f));
            else if (node->model == &_desnaNogaGore) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.3f, duzinaNadkoljenice, 0.35f));
            else if (node->model == &_desnoKoljeno) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.16f));
            else if (node->model == &_desnaNogaDole) finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(0.25f, duzinaPotkoljenice, 0.3f));

            drawComponent(*node->model, finalDrawMatrix);
        }

        // Rekurzivno pozovi za svu djecu
        TreeNode* currentChild = node->child;
        while (currentChild != nullptr) {
            traverseAndDraw(currentChild, currentModelMatrix);
            currentChild = currentChild->sibling;
        }
    }

    // Glavna setup funkcija, poziva se jednom u onInit
    void setup()
    {
        // Ucitaj teksture i dobij njihove OpenGL ID-jeve
        GLuint shirtTextureID = loadJPG(_shirtTex);
        GLuint skinTextureID = loadJPG(_skinTex);
        GLuint jeansTextureID = loadJPG(_jeansTex);

        // Generiraj geometriju za primitivne oblike
        ShapeData kockaData = ShapeGenerator::createCube();
        ShapeData sferaData = ShapeGenerator::createSphere(1.0f, 32, 16);
        ShapeData cilindarData = ShapeGenerator::createCylinder(1.0f, 0.5f, 16);
        ShapeData polusferaData = ShapeGenerator::createHemisphere(1.0f, 0.5f, 16);

        // Kreiraj sve komponente modela, dodjeljujuci im geometriju i teksturu
        createAndLoadModel(cilindarData, shirtTextureID, _trup);
        createAndLoadModel(sferaData, skinTextureID, _glava);

        createAndLoadModel(kockaData, jeansTextureID, _lijevaNogaGore);
        createAndLoadModel(kockaData, jeansTextureID, _lijevaNogaDole);
        createAndLoadModel(kockaData, jeansTextureID, _desnaNogaGore);
        createAndLoadModel(kockaData, jeansTextureID, _desnaNogaDole);

        createAndLoadModel(kockaData, shirtTextureID, _lijevaRukaGore);
        createAndLoadModel(kockaData, skinTextureID, _lijevaRukaDole);
        createAndLoadModel(kockaData, shirtTextureID, _desnaRukaGore);
        createAndLoadModel(kockaData, skinTextureID, _desnaRukaDole);

        createAndLoadModel(sferaData, shirtTextureID, _lijevoRame);
        createAndLoadModel(sferaData, shirtTextureID, _desnoRame);
        createAndLoadModel(sferaData, skinTextureID, _lijeviLakat);
        createAndLoadModel(sferaData, skinTextureID, _desniLakat);
        createAndLoadModel(polusferaData, shirtTextureID, _lijeviKuk);
        createAndLoadModel(polusferaData, jeansTextureID, _desniKuk);
        createAndLoadModel(sferaData, jeansTextureID, _lijevoKoljeno);
        createAndLoadModel(sferaData, jeansTextureID, _desnoKoljeno);

        // Uzmi lokacije uniformi iz shadera
        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");
        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");

        // Pozovi funkciju koja ce sve sloziti u hijerarhijsko stablo
        setupSceneGraph();
    }

protected:
    void onInit() override
    {
        if (!setupShaders()) { assert(false); return; }

        // Osnovne OpenGL postavke
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        setup(); // Pozovi glavnu setup funkciju

        _lastFrameTime = std::chrono::steady_clock::now();
        setFocus();
    }

    void onResize(const gui::Size& newSize) override
    {
        View::onResize(newSize);
        _kamera.setViewportSize(newSize);
    }

    // Azurira stanje scena svaki frame, prije crtanja
    bool prepareNextFrame() override
    {
        // Izracunaj vrijeme proteklo od proslog frame-a
        auto currentTime = std::chrono::steady_clock::now();
        float time = std::chrono::duration<float>(currentTime - _lastFrameTime).count();
        _lastFrameTime = currentTime;

        // 1. Kretanje kamere i rotacija lika
        if (_kameraZoomIn) _kameraUdaljenost -= _kameraBrzinaZuma * time;
        if (_kameraZoomOut) _kameraUdaljenost += _kameraBrzinaZuma * time;
        _kameraUdaljenost = std::max(3.0f, std::min(_kameraUdaljenost, 50.0f));

        float trenutnaBrzinaRotacije = 3.0f;
        if (_rotacijaLijevo) _humanoidUgaoRotacije += trenutnaBrzinaRotacije * time;
        if (_rotacijaDesno) _humanoidUgaoRotacije -= trenutnaBrzinaRotacije * time;

        // Izracunaj pomak na osnovu inputa
        bool isMoving = _kretanjeNaprijed || _kretanjeNazad;
        float moveStep = 0.0f;
        float trenutnaBrzinaHoda = _baseBrzinaHoda;
        bool isSprinting = _kretanjeNaprijed && _sprintAktivan;

        if (_kretanjeNaprijed) {
            float brzina = isSprinting ? _baseBrzinaKretanja * _sprintFaktor : _baseBrzinaKretanja;
            moveStep += brzina;
        }
        if (_kretanjeNazad) {
            moveStep -= _baseBrzinaKretanja;
        }

        if (isSprinting) {
            trenutnaBrzinaHoda *= _sprintFaktor;
        }

        _humanoidPozicijaX += moveStep * -glm::cos(_humanoidUgaoRotacije) * time;
        _humanoidPozicijaZ += moveStep * glm::sin(_humanoidUgaoRotacije) * time;

        // 2. Azuriranje pozicije i rotacije cijelog lika
        glm::mat4 rootTransform = glm::mat4(1.0f);
        rootTransform = glm::translate(rootTransform, glm::vec3(_humanoidPozicijaX, 0.0f, _humanoidPozicijaZ));
        rootTransform = glm::rotate(rootTransform, _humanoidUgaoRotacije - glm::pi<float>() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        _humanoidRoot->localTransform = rootTransform;

        // 3. ANIMACIJA HODA
        if (isMoving) {
            _ugaoHoda += trenutnaBrzinaHoda * time;
        }
        else {
            // Glatko zaustavljanje animacije
            float ciljniUgao = round(_ugaoHoda / glm::pi<float>()) * glm::pi<float>();
            _ugaoHoda += (ciljniUgao - _ugaoHoda) * (8.0f * time);
        }

        // Dinamicki faktori za realisticniji hod i trcanje
        float faktorSavijanjaKoljena;
        float faktorZamahKuka;
        if (isSprinting) {
            faktorZamahKuka = 0.8f;
            faktorSavijanjaKoljena = 1.2f;
        }
        else {
            faktorZamahKuka = 0.5f;
            faktorSavijanjaKoljena = 0.6f;
        }

        float ugaoHoda = sin(_ugaoHoda) * faktorZamahKuka;
        float ugaoKoljena = glm::max(0.0f, -sin(_ugaoHoda) * faktorSavijanjaKoljena);
        float ugaoKoljenaSuprotno = glm::max(0.0f, sin(_ugaoHoda) * faktorSavijanjaKoljena);

        // Primjena rotacija na zglobove
        _nodeMap["left_shoulder"]->localTransform = _nodeMap["left_shoulder"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoHoda, glm::vec3(0.9f, 0.0f, 0.0f));
        _nodeMap["left_elbow"]->localTransform = _nodeMap["left_elbow"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_shoulder"]->localTransform = _nodeMap["right_shoulder"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_elbow"]->localTransform = _nodeMap["right_elbow"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["left_hip"]->localTransform = _nodeMap["left_hip"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["left_knee"]->localTransform = _nodeMap["left_knee"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_hip"]->localTransform = _nodeMap["right_hip"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_knee"]->localTransform = _nodeMap["right_knee"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));

        // 4. ANIMACIJA MAHANJA (sa glatkim prijelazom)
        float brzinaPodizanja = 2.5f;
        if (_mahanjeAktivno) {
            _progressPodizanjaRuke = std::min(1.0f, _progressPodizanjaRuke + brzinaPodizanja * time);
        }
        else {
            _progressPodizanjaRuke = std::max(0.0f, _progressPodizanjaRuke - brzinaPodizanja * time);
        }

        if (_progressPodizanjaRuke > 0.0f) {
            // Interpolacija između hodanja i mahanja za glatki prijelaz
            glm::quat rotacijaRameHoda = glm::angleAxis(-ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat rotacijaRameMahanje = glm::angleAxis(-glm::pi<float>() / 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat finalnaRotacijaRame = glm::slerp(rotacijaRameHoda, rotacijaRameMahanje, _progressPodizanjaRuke);
            _nodeMap["right_shoulder"]->localTransform = _nodeMap["right_shoulder"]->baseTransform * glm::mat4_cast(finalnaRotacijaRame);

            glm::quat rotacijaLakatHoda = glm::angleAxis(-ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
            if (_progressPodizanjaRuke > 0.9f) {
                _ugaoMahanja += 4.0f * time;
            }
            glm::mat4 fiksnaRotacija = glm::rotate(glm::mat4(1.0f), -glm::pi<float>() / 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 rotacijaMahanja = glm::rotate(glm::mat4(1.0f), 0.5f * sin(_ugaoMahanja), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotacijaLakatMahanje = glm::quat_cast(rotacijaMahanja * fiksnaRotacija);
            glm::quat finalnaRotacijaLakat = glm::slerp(rotacijaLakatHoda, rotacijaLakatMahanje, _progressPodizanjaRuke);
            _nodeMap["right_elbow"]->localTransform = _nodeMap["right_elbow"]->baseTransform * glm::mat4_cast(finalnaRotacijaLakat);
        }

        return true;
    }

    // Crtanje scene
    void onDraw(const gui::Rect& rect) override
    {
        // Postavljanje kamere
        glm::vec3 targetPozicija = glm::vec3(_humanoidPozicijaX, 1.5f, _humanoidPozicijaZ);
        float visinaKamere = 5.0f;
        float camX = targetPozicija.x + _kameraUdaljenost * cos(_kameraUgaoOkoLika);
        float camZ = targetPozicija.z + _kameraUdaljenost * sin(_kameraUgaoOkoLika);
        glm::vec3 cameraPosition = glm::vec3(camX, visinaKamere, camZ);
        _viewMatrix = glm::lookAt(cameraPosition, targetPozicija, glm::vec3(0, 1, 0));
        _perspectiveMatrix = _kamera.getPerspectiveMatrix();

        // ciscenje ekrana i aktivacija shadera
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(_mainPassProgramID);

        // Pokretanje rekurzivnog crtanja od korijena stabla
        traverseAndDraw(_humanoidRoot, glm::mat4(1.0f));
    }

    // Obrada pritiska na tipku
    bool onKeyPressed(const gui::Key& key) override
    {
        _sprintAktivan = key.isShiftPressed();

        auto c = key.getChar();
        if (c == 'w' || c == 'W') _kretanjeNaprijed = true;
        if (c == 's' || c == 'S') _kretanjeNazad = true;
        if (c == 'a' || c == 'A') _rotacijaLijevo = true;
        if (c == 'd' || c == 'D') _rotacijaDesno = true;
        if (c == 'q' || c == 'Q') _kameraZoomOut = true;
        if (c == 'e' || c == 'E') _kameraZoomIn = true;

        if (c == 'm' || c == 'M') _mahanjeAktivno = !_mahanjeAktivno;

        return true;
    }

    // Obrada otpustanja tipke
    bool onKeyReleased(const gui::Key& key) override
    {
        _sprintAktivan = key.isShiftPressed();

        auto c = key.getChar();
        if (c == 'w' || c == 'W') _kretanjeNaprijed = false;
        if (c == 's' || c == 'S') _kretanjeNazad = false;
        if (c == 'a' || c == 'A') _rotacijaLijevo = false;
        if (c == 'd' || c == 'D') _rotacijaDesno = false;
        if (c == 'q' || c == 'Q') _kameraZoomOut = false;
        if (c == 'e' || c == 'E') _kameraZoomIn = false;

        return true;
    }

public:
    // Konstruktor
    MainView()
        : _kamera(gui::Size(1280, 720)),
        _shirtTex(":Shirt"),
        _skinTex(":koza"),
        _jeansTex(":Jeans")
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3);
        createContext(reqContext, { gui::InputDevice::Event::Keyboard });
    }

    // Destruktor
    ~MainView() {
        delete _humanoidRoot;
    }
};