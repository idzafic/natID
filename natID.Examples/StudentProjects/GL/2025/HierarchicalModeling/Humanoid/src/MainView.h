#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include "Camera.h"
#include "objloader.h" // Uključujemo vaš OBJ LOADER

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

// Struktura koja čuva sve OpenGL podatke i dimenzije za jedan vidljivi dio modela
struct ModelComponent
{
    GLuint vao = 0;
    GLuint vbo_vertices = 0;
    GLuint vbo_normals = 0;
    GLuint vbo_uvs = 0;
    td::UINT4 vertexCount = 0;
    GLuint textureID = 0;

    glm::vec3 dimensions; // x = širina, y = visina, z = dubina
};

// Čvor u scenoskom grafu, definira hijerarhiju i transformacije
struct TreeNode {
    glm::mat4 localTransform;
    glm::mat4 baseTransform;
    ModelComponent* model = nullptr;
    TreeNode* parent = nullptr;
    TreeNode* child = nullptr;
    TreeNode* sibling = nullptr;
    std::vector<TreeNode*> children;
    ~TreeNode() { for (auto c : children) delete c; }
    void addChild(TreeNode* newChild) {
        newChild->parent = this;
        if (child == nullptr) { child = newChild; }
        else {
            TreeNode* lastSibling = child;
            while (lastSibling->sibling != nullptr) { lastSibling = lastSibling->sibling; }
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

    // Geometrija za SVE dijelove robota
    ModelComponent _trup, _glava;
    ModelComponent _rameL, _nadlakticaL, _lakatL, _podlakticaL;
    ModelComponent _rameD, _nadlakticaD, _lakatD, _podlakticaD;
    ModelComponent _kukL, _natkoljenicaL, _koljenoL, _potkoljenicaL;
    ModelComponent _kukD, _natkoljenicaD, _koljenoD, _potkoljenicaD;

    // Tekstura
    gui::Texture _robotTextureObj; // Pomoćni objekt za prosljeđivanje imena resursa teksture

    // Scenski graf
    TreeNode* _humanoidRoot = nullptr;
    std::map<std::string, TreeNode*> _nodeMap;

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
    float _progressPodizanjaRuke = 0.0f; // Za glatku tranziciju mahanja (0.0 = spuštena, 1.0 = podignuta)

    // Varijable za kameru
    bool _kameraZoomIn = false;
    bool _kameraZoomOut = false;
    float _kameraUdaljenost = 60.0f;
    const float _kameraBrzinaZuma = 15.0f;
    float _kameraUgaoOkoLika = 0.0f;

    std::chrono::steady_clock::time_point _lastFrameTime;

private:
    bool setupShaders()
    {
        _mainPassProgramID = linkProgram(":mainV", ":mainF");
        assert(_mainPassProgramID > 0);
        return true;
    }

    // Učitava .obj, automatski centrira pivot i računa dimenzije modela
    void createAndLoadModel(const char* objResID, GLuint textureID, ModelComponent& component)
    {
        component.textureID = textureID;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        td::String resPath = gui::getResFileName(objResID);
        bool res = loadOBJ(resPath.c_str(), false, vertices, uvs, normals);
        if (!res) {
            mu::dbgLog("GRESKA: Nije uspjelo ucitavanje modela: %s.", resPath.c_str());
            assert(false);
            //__debugbreak();
            return;
        }

        // Automatsko centriranje pivota i izračunavanje dimenzija
        glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::lowest());
        for (const auto& vertex : vertices) {
            minAABB = glm::min(minAABB, vertex);
            maxAABB = glm::max(maxAABB, vertex);
        }
        glm::vec3 center = (maxAABB + minAABB) * 0.5f;

        for (auto& vertex : vertices) {
            vertex -= center;
        }
        component.dimensions = maxAABB - minAABB;
        component.vertexCount = (td::UINT4)vertices.size();

        // Slanje podataka u OpenGL buffere
        glGenVertexArrays(1, &component.vao);
        glBindVertexArray(component.vao);

        glGenBuffers(1, &component.vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &component.vbo_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &component.vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindVertexArray(0);
    }

    // Iscrtava jednu komponentu
    void drawComponent(const ModelComponent& component, const glm::mat4& modelMatrix)
    {
        glm::mat4 MVP = _perspectiveMatrix * _viewMatrix * modelMatrix;
        glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(component.vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, component.textureID);
        glUniform1i(_textureIDInShader, 0);

        // Koristimo glDrawArrays jer naš OBJ loader ne vraća indekse
        glDrawArrays(GL_TRIANGLES, 0, component.vertexCount);

        glBindVertexArray(0);
    }

    // Ovdje ručno podešavamo početne pozicije i hijerarhiju dijelova robota
    void setupSceneGraph() {
        delete _humanoidRoot;
        _nodeMap.clear();


        float faktorSkaliranja = 20.0f;
        // --- VRIJEDNOSTI SE SADA ČITAJU IZ MODELA ---
        float visinaTrupa = _trup.dimensions.y * faktorSkaliranja;
        float visinaGlave = _glava.dimensions.y * faktorSkaliranja;

        // Varijable za ručno podešavanje pozicija
        float pomakGlaveY = ((visinaTrupa / 2.0f) + (visinaGlave / 2.0f)) * 0.95f;
        float pomakRamenaX = 2.7f;
        float pomakRamenaY = 2.78f;
        float pomakRamenaZ = -0.35f;

        // KORIJEN I TRUP
        _humanoidRoot = new TreeNode();
        _nodeMap["root"] = _humanoidRoot;
        auto* torsoNode = new TreeNode();
        torsoNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        torsoNode->model = &_trup;
        _humanoidRoot->addChild(torsoNode);
        _nodeMap["torso"] = torsoNode;

        // GLAVA
        auto* headNode = new TreeNode();
        headNode->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.09f, pomakGlaveY, 0.25f));
        headNode->model = &_glava;
        torsoNode->addChild(headNode);
        _nodeMap["head"] = headNode;

        // --- LIJEVA RUKA ---
        auto* L_Shoulder = new TreeNode();
        L_Shoulder->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(pomakRamenaX, pomakRamenaY, pomakRamenaZ));
        L_Shoulder->model = &_rameL;
        torsoNode->addChild(L_Shoulder);
        _nodeMap["left_shoulder"] = L_Shoulder;

        auto* L_UpperArm = new TreeNode();
        glm::mat4 nadlakticaTransform = glm::mat4(1.0f);
        nadlakticaTransform = glm::translate(nadlakticaTransform, glm::vec3(0.2f, -1.5f, 0.0f));
        nadlakticaTransform = glm::rotate(nadlakticaTransform, glm::radians(-75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        L_UpperArm->baseTransform = nadlakticaTransform;
        L_UpperArm->model = &_nadlakticaL;
        L_Shoulder->addChild(L_UpperArm);
        _nodeMap["left_upper_arm"] = L_UpperArm;

        auto* L_Elbow = new TreeNode();
        L_Elbow->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, -0.5f, 0.1f));
        L_Elbow->model = &_lakatL;
        L_UpperArm->addChild(L_Elbow);
        _nodeMap["left_elbow"] = L_Elbow;

        auto* L_LowerArm = new TreeNode();
        L_LowerArm->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(4.8f, -0.57f, 0.34f));
        L_LowerArm->model = &_podlakticaL;
        L_Elbow->addChild(L_LowerArm);
        _nodeMap["left_lower_arm"] = L_LowerArm;

        // --- DESNA RUKA ---
        auto* R_Shoulder = new TreeNode();
        R_Shoulder->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-2.9f, 2.92f, -0.32f));
        R_Shoulder->model = &_rameD;
        torsoNode->addChild(R_Shoulder);
        _nodeMap["right_shoulder"] = R_Shoulder;

        auto* R_UpperArm = new TreeNode();
        glm::mat4 nadlakticaTransformD = glm::mat4(1.0f);
        nadlakticaTransformD = glm::translate(nadlakticaTransformD, glm::vec3(-0.2f, -1.65f, 0.0f));
        nadlakticaTransformD = glm::rotate(nadlakticaTransformD, glm::radians(75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        R_UpperArm->baseTransform = nadlakticaTransformD;
        R_UpperArm->model = &_nadlakticaD;
        R_Shoulder->addChild(R_UpperArm);
        _nodeMap["right_upper_arm"] = R_UpperArm;

        auto* R_Elbow = new TreeNode();
        R_Elbow->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-2.6f, -0.5f, 0.05f));
        R_Elbow->model = &_lakatD;
        R_UpperArm->addChild(R_Elbow);
        _nodeMap["right_elbow"] = R_Elbow;

        auto* R_LowerArm = new TreeNode();
        R_LowerArm->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-4.75f, -0.57f, 0.34f));
        R_LowerArm->model = &_podlakticaD;
        R_Elbow->addChild(R_LowerArm);
        _nodeMap["right_lower_arm"] = R_LowerArm;

        // --- LIJEVA NOGA ---
        auto* L_Hip = new TreeNode();
        L_Hip->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.66f, -5.25f, 0.21f));
        L_Hip->model = &_kukL;
        torsoNode->addChild(L_Hip);
        _nodeMap["left_hip"] = L_Hip;

        auto* L_UpperLeg = new TreeNode();
        L_UpperLeg->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.9f, -3.35f, -0.22f));
        L_UpperLeg->model = &_natkoljenicaL;
        L_Hip->addChild(L_UpperLeg);
        _nodeMap["left_upper_leg"] = L_UpperLeg;

        auto* L_Knee = new TreeNode();
        L_Knee->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -4.4, -0.35f));
        L_Knee->model = &_koljenoL;
        L_UpperLeg->addChild(L_Knee);
        _nodeMap["left_knee"] = L_Knee;

        auto* L_LowerLeg = new TreeNode();
        L_LowerLeg->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -4.5, 1.0f));
        L_LowerLeg->model = &_potkoljenicaL;
        L_Knee->addChild(L_LowerLeg);
        _nodeMap["left_lower_leg"] = L_LowerLeg;

        // --- DESNA NOGA ---
        auto* R_Hip = new TreeNode();
        R_Hip->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.66f, -5.25f, 0.21f));
        R_Hip->model = &_kukD;
        torsoNode->addChild(R_Hip);
        _nodeMap["right_hip"] = R_Hip;

        auto* R_UpperLeg = new TreeNode();
        R_UpperLeg->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.9f, -3.35f, -0.22f));
        R_UpperLeg->model = &_natkoljenicaD;
        R_Hip->addChild(R_UpperLeg);
        _nodeMap["right_upper_leg"] = R_UpperLeg;

        auto* R_Knee = new TreeNode();
        R_Knee->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, -4.4, -0.35f));
        R_Knee->model = &_koljenoD;
        R_UpperLeg->addChild(R_Knee);
        _nodeMap["right_knee"] = R_Knee;

        auto* R_LowerLeg = new TreeNode();
        R_LowerLeg->baseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, -4.5, 1.0f));
        R_LowerLeg->model = &_potkoljenicaD;
        R_Knee->addChild(R_LowerLeg);
        _nodeMap["right_lower_leg"] = R_LowerLeg;

        // Postavi početne transformacije za sve čvorove
        for (auto const& [key, val] : _nodeMap) {
            if (val) val->localTransform = val->baseTransform;
        }
    }

    // Rekurzivno iscrtavanje stabla
    void traverseAndDraw(TreeNode* node, const glm::mat4& parentModelMatrix) {
        if (!node) return;

        glm::mat4 currentModelMatrix = parentModelMatrix * node->localTransform;

        if (node->model) {
            glm::mat4 finalDrawMatrix = currentModelMatrix;

            // Globalno skaliranje i rotacija se primjenjuju na SVE dijelove ovdje
            finalDrawMatrix = glm::scale(finalDrawMatrix, glm::vec3(20.0f));

            drawComponent(*node->model, finalDrawMatrix);
        }

        TreeNode* currentChild = node->child;
        while (currentChild != nullptr) {
            traverseAndDraw(currentChild, currentModelMatrix);
            currentChild = currentChild->sibling;
        }
    }

    // Glavna setup funkcija
    void setup()
    {
        // 1. Učitaj teksturu JEDNOM i dobij njen GLuint ID
        GLuint robotTextureID = loadJPG(_robotTextureObj);

        // 2. Učitaj SVE modele - imena resursa moraju odgovarati onima u XML-u
        createAndLoadModel(":Tijelo", robotTextureID, _trup);
        createAndLoadModel(":Glava", robotTextureID, _glava);

        createAndLoadModel(":Lijevo_rame", robotTextureID, _rameL);
        createAndLoadModel(":Lijeva_nadlaktica", robotTextureID, _nadlakticaL);
        createAndLoadModel(":Lijevi_lakat", robotTextureID, _lakatL);
        createAndLoadModel(":Lijeva_podlaktica", robotTextureID, _podlakticaL);

        createAndLoadModel(":Desno_rame", robotTextureID, _rameD);
        createAndLoadModel(":Desna_nadlaktica", robotTextureID, _nadlakticaD);
        createAndLoadModel(":Desni_lakat", robotTextureID, _lakatD);
        createAndLoadModel(":Desna_podlaktica", robotTextureID, _podlakticaD);

        createAndLoadModel(":Lijevi_kuk", robotTextureID, _kukL);
        createAndLoadModel(":Lijeva_nadkoljenca", robotTextureID, _natkoljenicaL);
        createAndLoadModel(":Lijevo_koljeno", robotTextureID, _koljenoL);
        createAndLoadModel(":Lijeva_podkoljenica", robotTextureID, _potkoljenicaL);

        createAndLoadModel(":Desni_kuk", robotTextureID, _kukD);
        createAndLoadModel(":Desna_nadkoljenica", robotTextureID, _natkoljenicaD);
        createAndLoadModel(":Desno_koljeno", robotTextureID, _koljenoD);
        createAndLoadModel(":Desna_podkoljenica", robotTextureID, _potkoljenicaD);

        // 3. Uzmi lokacije uniformi iz shadera
        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");
        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");

        // 4. Pozovi funkciju koja će sve ovo složiti u hijerarhijsko stablo
        setupSceneGraph();
    }

protected:
    void onInit() override
    {
        if (!setupShaders()) { assert(false); return; }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        setup();

        _lastFrameTime = std::chrono::steady_clock::now();
        setFocus();
    }

    void onResize(const gui::Size& newSize) override
    {
        View::onResize(newSize);
        _kamera.setViewportSize(newSize);
    }

    // Ažurira stanje scena svaki frame
    bool prepareNextFrame() override
    {
        auto currentTime = std::chrono::steady_clock::now();
        float time = std::chrono::duration<float>(currentTime - _lastFrameTime).count();
        _lastFrameTime = currentTime;

        // 1. Kretanje kamere i rotacija lika
        if (_kameraZoomIn) _kameraUdaljenost -= _kameraBrzinaZuma * time;
        if (_kameraZoomOut) _kameraUdaljenost += _kameraBrzinaZuma * time;
        _kameraUdaljenost = std::max(5.0f, std::min(_kameraUdaljenost, 95.0f));

        float trenutnaBrzinaRotacije = 3.0f;
        if (_rotacijaLijevo) _humanoidUgaoRotacije += trenutnaBrzinaRotacije * time;
        if (_rotacijaDesno) _humanoidUgaoRotacije -= trenutnaBrzinaRotacije * time;

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

        // 2. Ažuriranje pozicije i rotacije cijelog lika
        glm::mat4 rootTransform = glm::mat4(1.0f);
        rootTransform = glm::translate(rootTransform, glm::vec3(_humanoidPozicijaX, 0.0f, _humanoidPozicijaZ));
        rootTransform = glm::rotate(rootTransform, _humanoidUgaoRotacije - glm::pi<float>() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        _humanoidRoot->localTransform = rootTransform;

        // 3. ANIMACIJA HODA
        if (isMoving) {
            _ugaoHoda += trenutnaBrzinaHoda * time;
        }
        else {
            float ciljniUgao = round(_ugaoHoda / glm::pi<float>()) * glm::pi<float>();
            _ugaoHoda += (ciljniUgao - _ugaoHoda) * (8.0f * time);
        }

        // Dinamički faktori za realističniji hod i trčanje
        float faktorSavijanjaKoljena;
        float faktorZamahKuka;
        if (isSprinting) {
            faktorZamahKuka = 1.2f;
            faktorSavijanjaKoljena = 1.5f;
        }
        else {
            faktorZamahKuka = 0.7f;
            faktorSavijanjaKoljena = 1.3f;
        }

        float ugaoHoda = sin(_ugaoHoda) * faktorZamahKuka;
        float ugaoKoljena = glm::max(0.0f, -sin(_ugaoHoda) * faktorSavijanjaKoljena);
        float ugaoKoljenaSuprotno = glm::max(0.0f, sin(_ugaoHoda) * faktorSavijanjaKoljena);
        float ugaoLakta = glm::max(0.0f, -sin(_ugaoHoda) * (faktorSavijanjaKoljena - 0.8f));
        float ugaoLaktaSuprotno = glm::max(0.0f, sin(_ugaoHoda) * (faktorSavijanjaKoljena - 0.8f));

        // Primjena rotacija na zglobove
        _nodeMap["left_shoulder"]->localTransform = _nodeMap["left_shoulder"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_shoulder"]->localTransform = _nodeMap["right_shoulder"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["left_elbow"]->localTransform = _nodeMap["left_elbow"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoLakta, glm::vec3(0.0f, -1.0f, 0.0f));
        _nodeMap["right_elbow"]->localTransform = _nodeMap["right_elbow"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoLaktaSuprotno, glm::vec3(0.0f, 1.0f, 0.0f));
        _nodeMap["left_hip"]->localTransform = _nodeMap["left_hip"]->baseTransform * glm::rotate(glm::mat4(1.0f), -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_hip"]->localTransform = _nodeMap["right_hip"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["left_knee"]->localTransform = _nodeMap["left_knee"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        _nodeMap["right_knee"]->localTransform = _nodeMap["right_knee"]->baseTransform * glm::rotate(glm::mat4(1.0f), ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));

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
            glm::mat4 okretanjeLakta = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 rotacijaMahanja = glm::rotate(glm::mat4(1.0f), 0.5f * sin(_ugaoMahanja), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::quat rotacijaLakatMahanje = glm::quat_cast(rotacijaMahanja * okretanjeLakta * fiksnaRotacija);
            glm::quat finalnaRotacijaLakat = glm::slerp(rotacijaLakatHoda, rotacijaLakatMahanje, _progressPodizanjaRuke);
            _nodeMap["right_elbow"]->localTransform = _nodeMap["right_elbow"]->baseTransform * glm::mat4_cast(finalnaRotacijaLakat);
        }

        return true;
    }

    void onDraw(const gui::Rect& rect) override
    {
        glm::vec3 targetPozicija = glm::vec3(_humanoidPozicijaX, 7.0f, _humanoidPozicijaZ);
        float visinaKamere = 15.0f;
        float camX = targetPozicija.x + _kameraUdaljenost * cos(_kameraUgaoOkoLika);
        float camZ = targetPozicija.z + _kameraUdaljenost * sin(_kameraUgaoOkoLika);
        glm::vec3 cameraPosition = glm::vec3(camX, visinaKamere, camZ);
        _viewMatrix = glm::lookAt(cameraPosition, targetPozicija, glm::vec3(0, 1, 0));
        _perspectiveMatrix = _kamera.getPerspectiveMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(_mainPassProgramID);

        traverseAndDraw(_humanoidRoot, glm::mat4(1.0f));
    }

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
    MainView()
        : _kamera(gui::Size(1280, 720)),
        _robotTextureObj(":robotColor")
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3);
        createContext(reqContext, { gui::InputDevice::Event::Keyboard });
    }
    ~MainView() {
        delete _humanoidRoot;
    }
};
