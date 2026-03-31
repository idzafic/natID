#include "HeightMap.h"

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};


class Tenk {

    GLuint _vertexbufferTenk;
    GLuint _elementbufferTenk;
    GLuint _uvbufferTenk;
    GLuint _normalbufferTenk;

    gui::Texture _textTank;
    GLuint _textureTank;

    GLuint _vertexbufferRemains;
    GLuint _elementbufferRemains;
    GLuint _uvbufferRemains;
    GLuint _normalbufferRemains;

    gui::Texture _textRemains;
    GLuint _textureRemains;

    td::UINT4 _indicesCntTenk,_indicesCntRemains;

    GLuint _vertexbufferWheels;
    GLuint _elementbufferWheels;
    GLuint _uvbufferWheels;
    GLuint _normalbufferWheels;

    GLuint _vertexbufferWheels2;
    GLuint _elementbufferWheels2;
    GLuint _uvbufferWheels2;
    GLuint _normalbufferWheels2;

    gui::Texture _textWheels;
    GLuint _textureWheels;

    td::UINT4 _indicesCntWheels;
    td::UINT4 _indicesCntWheels2;


    float naprijedNazad;
    float lijevoDesno;
    float goreDole;
    float rotacija;


    GLuint _vertexbufferBarrel, _uvbufferBarrel, _normalbufferBarrel, _elementbufferBarrel;
    GLuint _vertexbufferShell, _uvbufferShell, _normalbufferShell, _elementbufferShell;
    GLuint _vertexbufferTurret, _uvbufferTurret, _normalbufferTurret, _elementbufferTurret;
    GLuint _vertexbufferTracksL[7], _uvbufferTracksL[7], _normalbufferTracksL[7], _elementbufferTracksL[7];
    GLuint _vertexbufferTracksD[7], _uvbufferTracksD[7], _normalbufferTracksD[7], _elementbufferTracksD[7];

    td::UINT4 _indicesCntBarrel, _indicesCntTurret,_indicesCntShell,_indicesCntTracksL[7], _indicesCntTracksD[7];

    gui::Texture _textBarrel, _textTurret,_textTracks,_textShell;
    GLuint _textureBarrel, _textureTurret,_textureTracks,_textureShell;

    float turretRotacija;
    float barrelRotacija;

    float brzinaCijevi;

    float _trackTextureOffset = 0.0f;

    bool seRotira = false;

    int animL = 0, animD = 0;

    const char* objTracksL[7] = { ":track2LObj",":track3LObj" ,":track4LObj" ,":track5LObj" ,
    ":track6LObj" ,":track7LObj" , ":track8LObj"  };

    const char* objTracksD[7] = {":track2DObj",":track3DObj" ,":track4DObj" ,":track5DObj" ,
    ":track6DObj" ,":track7DObj" , ":track8DObj"  };

    float pucanjeStep = 0;

    bool pucanjeAktivno = false;

    HeightMap _heightMap;

    float _animShell = 0;

    bool _crtajShell = false;


    float _shellInitialx, _shellInitialy, _shellInitialz;

    float _shellInitialRotation1, _shellInitialRotation2;


    float Vshell = 4.0f;  // brzina projektila

    float g = 0.27f;    //gravitaciona konstanta

    float h, d , animT= 0;    //visina i predjeni put projektila


    glm::vec3 size = {3.0f,5.0f,5.0f};  // Width and height of tank body

    int health = 100;

    glm::vec3 pozicijaURavni = { 0.0f,0.0f,0.0f };

    glm::vec3 pozicijaShella = {-100.0f,-5.0f,0.0f };


public:

    void postaviTenk(GLuint& _textureIDInShader, td::UINT4& _mainPassProgramID, GLuint& _matrixID) {


        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);


        // Load the texture
        fo::fs::path homePath;
        mu::getHomePath(homePath);

//------------------------------------------za body----------------------------------------------

        _textureTank = loadJPG(_textTank);

        // Read .obj file
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        td::String roomWalls = gui::getResFileName(":tankObj");
        bool res = loadOBJ(roomWalls.c_str(), false, vertices, uvs, normals);



        std::vector<unsigned int> indices;
        std::vector<glm::vec3> indexed_vertices;
        std::vector<glm::vec2> indexed_uvs;
        std::vector<glm::vec3> indexed_normals;
        indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
        _indicesCntTenk = (td::UINT4)indices.size();

        glGenBuffers(1, &_vertexbufferTenk);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTenk);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbufferTenk);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTenk);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbufferTenk);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTenk);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbufferTenk);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTenk);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);




        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");

        //------------------------------------------za Remainse----------------------------------------------
        {
            _textureRemains = loadJPG(_textRemains);

            // Read .obj file
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> normals;

            td::String roomWalls = gui::getResFileName(":remainsObj");
            bool res = loadOBJ(roomWalls.c_str(), false, vertices, uvs, normals);



            std::vector<unsigned int> indices;
            std::vector<glm::vec3> indexed_vertices;
            std::vector<glm::vec2> indexed_uvs;
            std::vector<glm::vec3> indexed_normals;
            indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
            _indicesCntRemains = (td::UINT4)indices.size();

            glGenBuffers(1, &_vertexbufferRemains);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferRemains);
            glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_uvbufferRemains);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferRemains);
            glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_normalbufferRemains);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferRemains);
            glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_elementbufferRemains);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferRemains);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);




            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");
        }

        //------------------------------------------za Wheelse1----------------------------------------------
        {
            _textureWheels = loadJPG(_textWheels);

            // Read .obj file
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> normals;

            td::String roomWalls = gui::getResFileName(":wheelsObj");
            bool res = loadOBJ(roomWalls.c_str(), false, vertices, uvs, normals);



            std::vector<unsigned int> indices;
            std::vector<glm::vec3> indexed_vertices;
            std::vector<glm::vec2> indexed_uvs;
            std::vector<glm::vec3> indexed_normals;
            indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
            _indicesCntWheels = (td::UINT4)indices.size();

            glGenBuffers(1, &_vertexbufferWheels);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferWheels);
            glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_uvbufferWheels);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferWheels);
            glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_normalbufferWheels);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferWheels);
            glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_elementbufferWheels);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferWheels);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);




            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");
        }

        //------------------------------------------za Wheelse2----------------------------------------------
        {
            _textureWheels = loadJPG(_textWheels);

            // Read .obj file
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> normals;

            td::String roomWalls = gui::getResFileName(":wheels2Obj");
            bool res = loadOBJ(roomWalls.c_str(), false, vertices, uvs, normals);



            std::vector<unsigned int> indices;
            std::vector<glm::vec3> indexed_vertices;
            std::vector<glm::vec2> indexed_uvs;
            std::vector<glm::vec3> indexed_normals;
            indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
            _indicesCntWheels2 = (td::UINT4)indices.size();

            glGenBuffers(1, &_vertexbufferWheels2);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferWheels2);
            glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_uvbufferWheels2);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferWheels2);
            glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_normalbufferWheels2);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferWheels2);
            glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_elementbufferWheels2);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferWheels2);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);




            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");
        }

        //----------------------------------------za tracksL--------------------------------------------------------------------------------

        for (int i = 0; i < 7; i++) {

            _textureTracks = loadJPG(_textTracks);

            // Read .obj file
            std::vector<glm::vec3> tracksVerticesL;
            std::vector<glm::vec2> tracksUvsL;
            std::vector<glm::vec3> tracksNormalsL;

            td::String tracksL = gui::getResFileName(objTracksL[i]);
            bool resTracksL = loadOBJ(tracksL.c_str(), false, tracksVerticesL, tracksUvsL, tracksNormalsL);

            std::vector<unsigned int> tracksIndicesL;
            std::vector<glm::vec3> tracksIndexed_verticesL;
            std::vector<glm::vec2> tracksIndexed_uvsL;
            std::vector<glm::vec3> tracksIndexed_normalsL;
            indexVBO(tracksVerticesL, tracksUvsL, tracksNormalsL, tracksIndicesL, tracksIndexed_verticesL, tracksIndexed_uvsL, tracksIndexed_normalsL);
            _indicesCntTracksL[i] = (td::UINT4)tracksIndicesL.size();

            glGenBuffers(1, &_vertexbufferTracksL[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTracksL[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_verticesL.size() * sizeof(glm::vec3), &tracksIndexed_verticesL[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_uvbufferTracksL[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTracksL[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_uvsL.size() * sizeof(glm::vec2), &tracksIndexed_uvsL[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_normalbufferTracksL[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTracksL[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_normalsL.size() * sizeof(glm::vec3), &tracksIndexed_normalsL[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_elementbufferTracksL[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTracksL[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, tracksIndicesL.size() * sizeof(unsigned int), &tracksIndicesL[0], GL_STATIC_DRAW);




            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");

        }

        //----------------------------------------za tracksD--------------------------------------------------------------------------------

        for (int i = 0; i < 7; i++) {

            _textureTracks = loadJPG(_textTracks);

            // Read .obj file
            std::vector<glm::vec3> tracksVerticesD;
            std::vector<glm::vec2> tracksUvsD;
            std::vector<glm::vec3> tracksNormalsD;

            td::String tracksD = gui::getResFileName(objTracksD[i]);
            bool resTracksD = loadOBJ(tracksD.c_str(), false, tracksVerticesD, tracksUvsD, tracksNormalsD);

            std::vector<unsigned int> tracksIndicesD;
            std::vector<glm::vec3> tracksIndexed_verticesD;
            std::vector<glm::vec2> tracksIndexed_uvsD;
            std::vector<glm::vec3> tracksIndexed_normalsD;
            indexVBO(tracksVerticesD, tracksUvsD, tracksNormalsD, tracksIndicesD, tracksIndexed_verticesD, tracksIndexed_uvsD, tracksIndexed_normalsD);
            _indicesCntTracksD[i] = (td::UINT4)tracksIndicesD.size();

            glGenBuffers(1, &_vertexbufferTracksD[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTracksD[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_verticesD.size() * sizeof(glm::vec3), &tracksIndexed_verticesD[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_uvbufferTracksD[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTracksD[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_uvsD.size() * sizeof(glm::vec2), &tracksIndexed_uvsD[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_normalbufferTracksD[i]);
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTracksD[i]);
            glBufferData(GL_ARRAY_BUFFER, tracksIndexed_normalsD.size() * sizeof(glm::vec3), &tracksIndexed_normalsD[0], GL_STATIC_DRAW);

            glGenBuffers(1, &_elementbufferTracksD[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTracksD[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, tracksIndicesD.size() * sizeof(unsigned int), &tracksIndicesD[0], GL_STATIC_DRAW);




            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


        }


        //----------------------------------------za turret--------------------------------------------------------------------------------


        _textureTurret = loadJPG(_textTurret);

        // Read .obj file
        std::vector<glm::vec3> turretVertices;
        std::vector<glm::vec2> turretUvs;
        std::vector<glm::vec3> turretNormals;

        td::String turret = gui::getResFileName(":turretObj");
        bool resTurret = loadOBJ(turret.c_str(), false, turretVertices, turretUvs, turretNormals);

        std::vector<unsigned int> turretIndices;
        std::vector<glm::vec3> turretIndexed_vertices;
        std::vector<glm::vec2> turretIndexed_uvs;
        std::vector<glm::vec3> turretIndexed_normals;
        indexVBO(turretVertices, turretUvs, turretNormals, turretIndices, turretIndexed_vertices, turretIndexed_uvs, turretIndexed_normals);
        _indicesCntTurret = (td::UINT4)turretIndices.size();

        glGenBuffers(1, &_vertexbufferTurret);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTurret);
        glBufferData(GL_ARRAY_BUFFER, turretIndexed_vertices.size() * sizeof(glm::vec3), &turretIndexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbufferTurret);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTurret);
        glBufferData(GL_ARRAY_BUFFER, turretIndexed_uvs.size() * sizeof(glm::vec2), &turretIndexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbufferTurret);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTurret);
        glBufferData(GL_ARRAY_BUFFER, turretIndexed_normals.size() * sizeof(glm::vec3), &turretIndexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbufferTurret);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTurret);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, turretIndices.size() * sizeof(unsigned int), &turretIndices[0], GL_STATIC_DRAW);




        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


        //----------------------------------------za barrel--------------------------------------------------------------------------------


        _textureBarrel = loadJPG(_textBarrel);

        // Read .obj file
        std::vector<glm::vec3> barrelVertices;
        std::vector<glm::vec2> barrelUvs;
        std::vector<glm::vec3> barrelNormals;

        td::String barrel = gui::getResFileName(":barrelObj");
        bool resBarrel = loadOBJ(barrel.c_str(), false, barrelVertices, barrelUvs, barrelNormals);

        std::vector<unsigned int> barrelIndices;
        std::vector<glm::vec3> barrelIndexed_vertices;
        std::vector<glm::vec2> barrelIndexed_uvs;
        std::vector<glm::vec3> barrelIndexed_normals;
        indexVBO(barrelVertices, barrelUvs, barrelNormals, barrelIndices, barrelIndexed_vertices, barrelIndexed_uvs, barrelIndexed_normals);
        _indicesCntBarrel = (td::UINT4)barrelIndices.size();

        glGenBuffers(1, &_vertexbufferBarrel);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferBarrel);
        glBufferData(GL_ARRAY_BUFFER, barrelIndexed_vertices.size() * sizeof(glm::vec3), &barrelIndexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbufferBarrel);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbufferBarrel);
        glBufferData(GL_ARRAY_BUFFER, barrelIndexed_uvs.size() * sizeof(glm::vec2), &barrelIndexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbufferBarrel);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbufferBarrel);
        glBufferData(GL_ARRAY_BUFFER, barrelIndexed_normals.size() * sizeof(glm::vec3), &barrelIndexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbufferBarrel);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferBarrel);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, barrelIndices.size() * sizeof(unsigned int), &barrelIndices[0], GL_STATIC_DRAW);




        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");



        //----------------------------------------za shell--------------------------------------------------------------------------------


        _textureShell = loadJPG(_textShell);

        // Read .obj file
        std::vector<glm::vec3> shellVertices;
        std::vector<glm::vec2> shellUvs;
        std::vector<glm::vec3> shellNormals;

        td::String shell = gui::getResFileName(":shellObj");
        bool resShell = loadOBJ(shell.c_str(), false, shellVertices, shellUvs, shellNormals);

        std::vector<unsigned int> shellIndices;
        std::vector<glm::vec3> shellIndexed_vertices;
        std::vector<glm::vec2> shellIndexed_uvs;
        std::vector<glm::vec3> shellIndexed_normals;
        indexVBO(shellVertices, shellUvs, shellNormals, shellIndices, shellIndexed_vertices, shellIndexed_uvs, shellIndexed_normals);
        _indicesCntShell = (td::UINT4)shellIndices.size();

        glGenBuffers(1, &_vertexbufferShell);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferShell);
        glBufferData(GL_ARRAY_BUFFER, shellIndexed_vertices.size() * sizeof(glm::vec3), &shellIndexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_uvbufferShell);
        glBindBuffer(GL_ARRAY_BUFFER, _uvbufferShell);
        glBufferData(GL_ARRAY_BUFFER, shellIndexed_uvs.size() * sizeof(glm::vec2), &shellIndexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_normalbufferShell);
        glBindBuffer(GL_ARRAY_BUFFER, _normalbufferShell);
        glBufferData(GL_ARRAY_BUFFER, shellIndexed_normals.size() * sizeof(glm::vec3), &shellIndexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_elementbufferShell);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferShell);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shellIndices.size() * sizeof(unsigned int), &shellIndices[0], GL_STATIC_DRAW);




        _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");




    }


    void crtajTenk(GLuint& _textureIDInShader, td::UINT4& _mainPassProgramID, GLuint& _matrixID, MojaCamera& _camera) {


        glm::mat4 ProjectionMatrix = _camera.MojgetPerspectiveMatrix();
        glm::mat4 ViewMatrix = _camera.MojgetViewMatrix();


        if(health <= 0)
            //---------------------------------------crtanje remainsa----------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureRemains);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferRemains);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferRemains);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferRemains);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferRemains);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntRemains,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            return;
        }

       //---------------------------------------crtanje tenka----------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, {lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureTank);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTenk);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTenk);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTenk);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTenk);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntTenk,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }

        //---------------------------------------crtanje wheelsa1----------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureWheels);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferWheels);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferWheels);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferWheels);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferWheels);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntWheels,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }

        //---------------------------------------crtanje wheelsa2----------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureWheels);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferWheels2);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferWheels2);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferWheels2);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferWheels2);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntWheels2,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }

        //-----------------------------------------------crtanje tracksaL:-----------------------------------------------------

        {
            
            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureTracks);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTracksL[animL]);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTracksL[animL]);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTracksL[animL]);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTracksL[animL]);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntTracksL[animL],       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

        }

        //-----------------------------------------------crtanje tracksaD:-----------------------------------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

            ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureTracks);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTracksD[animD]);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTracksD[animD]);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTracksD[animD]);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTracksD[animD]);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntTracksD[animD],       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

        }

        

        //-----------------------------------------------crtanje turreta:-----------------------------------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

           // ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });  // ako treba pratiti rotaciju bodya    
            
            ModelMatrix = glm::rotate(ModelMatrix, turretRotacija, glm::vec3{ 0, 1, 0 });

            ModelMatrix = glm::translate(ModelMatrix, {-0.1f, 2.595f,1.1875f -1.78f});

           // ModelMatrix = glm::rotate(ModelMatrix, turretRotacija, glm::vec3{ 0, 1, 0 });

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureTurret);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferTurret);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferTurret);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferTurret);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferTurret);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntTurret,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }

        

        //-----------------------------------------------crtanje barrela:-----------------------------------------------------

        {

            glm::mat4 ModelMatrix = glm::mat4(1.0);

            ModelMatrix = glm::translate(ModelMatrix, { lijevoDesno,goreDole,naprijedNazad });

          //  ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

            ModelMatrix = glm::rotate(ModelMatrix, turretRotacija, glm::vec3{ 0, 1, 0 });

            ModelMatrix = glm::translate(ModelMatrix, { -0.1f, 2.595f,1.1875f - 1.78f });

//            ModelMatrix = glm::rotate(ModelMatrix, turretRotacija, glm::vec3{ 0, 1, 0 });

            ModelMatrix = glm::translate(ModelMatrix, { 0.0f , -0.328f, 1.8125f });

            ModelMatrix = glm::rotate(ModelMatrix, -barrelRotacija, glm::vec3{ 1.0f, 0.0f, 0.0f });

            ModelMatrix = glm::translate(ModelMatrix, { 0.0f, 0.0f, -sin(pucanjeStep) });

            

            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureBarrel);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferBarrel);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferBarrel);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferBarrel);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferBarrel);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntBarrel,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }

   //-------------------------------------------------------crtanje Shella------------------------------------------------------------

     if(_crtajShell)
     {

       glm::mat4 ModelMatrix = glm::mat4(1.0);

       ModelMatrix = glm::translate(ModelMatrix, { _shellInitialx, _shellInitialy ,_shellInitialz });

       //  ModelMatrix = glm::rotate(ModelMatrix, rotacija, glm::vec3{ 0, 1, 0 });

       ModelMatrix = glm::rotate(ModelMatrix, _shellInitialRotation1, glm::vec3{ 0, 1, 0 });

       ModelMatrix = glm::translate(ModelMatrix, { -0.1f, 2.595f,1.1875f - 1.78f });

       //            ModelMatrix = glm::rotate(ModelMatrix, turretRotacija, glm::vec3{ 0, 1, 0 });

       ModelMatrix = glm::translate(ModelMatrix, { 0.0f , -0.328f, 1.8125f + 5.0f});

       ModelMatrix = glm::rotate(ModelMatrix, -_shellInitialRotation2, glm::vec3{ 1.0f, 0.0f, 0.0f });

         h = (Vshell * sin(_shellInitialRotation2) * _animShell) - ((g * _animShell * _animShell) * 0.5);

         d = Vshell * cos(-_shellInitialRotation2) * _animShell;

       ModelMatrix = glm::translate(ModelMatrix, glm::vec3{ 0.0f, h , d });

       ModelMatrix = glm::rotate(ModelMatrix, _animShell * (2 * _shellInitialRotation2)/animT , glm::vec3{ 1.0f, 0.0f, 0.0f });

       pozicijaShella = glm::vec3(ModelMatrix * glm::vec4(0, 0, 0, 1));

       if (h < -2.4f - 90.0f * sin(_shellInitialRotation2) || _heightMap.collide(pozicijaShella))
       {
           _crtajShell = false;
           pozicijaShella = glm::vec3{ 0.0f, -5.0f, 0.0f };
       }

    //   ModelMatrix = glm::rotate(ModelMatrix, -_shellInitialRotation2, glm::vec3{ 1.0f, 0.0f, 0.0f });


            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            _textureIDInShader = glGetUniformLocation(_mainPassProgramID, "myTextureSampler");

            _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");


            // Send our transformation to the currently bound shader, 
            // in the "MVP" uniform
            glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, _textureMain);
            glBindTexture(GL_TEXTURE_2D, _textureShell);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(_textureIDInShader, 0);



            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, _vertexbufferShell);
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
            glBindBuffer(GL_ARRAY_BUFFER, _uvbufferShell);
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
            glBindBuffer(GL_ARRAY_BUFFER, _normalbufferShell);
            glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
            );

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbufferShell);

            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                _indicesCntShell,       // count
                GL_UNSIGNED_INT, // type
                (void*)0           // element array buffer offset
            );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            _animShell += 0.1;
        }






        if (pucanjeAktivno)
            pucanjeStep += 0.1;

        if (pucanjeStep >= 4 * atan(1))
        {
            pucanjeStep = 0;
            pucanjeAktivno = false;
        }

        seRotira = false;
    }


    void translirajTenk(glm::vec3 vektorTranslacije) {

        lijevoDesno += vektorTranslacije[0] * (cos(rotacija)) + vektorTranslacije[2] * sin(rotacija);

        goreDole += vektorTranslacije[1];
         
        naprijedNazad += vektorTranslacije[0] * (sin(rotacija)) + vektorTranslacije[2] * cos(rotacija);
        if (!seRotira) {
            if (vektorTranslacije[2] > 0)
            {
                // seKreceNaprijed 
                animL--;
                animD--;
                if (animL < 0)
                    animL = 6;

                if (animD < 0)
                    animD = 6;
            }
            else if (vektorTranslacije[2] < 0)
            {  //krece se nazad
                animL++;
                animD++;
                if (animL > 6)
                    animL = 0;

                if (animD > 6)
                    animD = 0;
            }
        }

        pozicijaURavni = glm::vec3{position().x,position().y,position().z};

    }

    void rotirajTenk(float Ugao) {
        
        rotacija += Ugao;

        seRotira = true;

        if (Ugao >= 0)
        {
            animL++;
            animD--;
        }
        else {
            animL--;
            animD++;
        }

        if (animL > 6)
            animL = 0;
        else if (animL < 0)
            animL = 6;
        if (animD > 6)
            animD = 0;
        else if (animD < 0)
            animD = 6;
    }

    void rotirajCijev(float Ugao) {

        turretRotacija = Ugao;

    }

    void podigniCijev(float Ugao) {

        barrelRotacija = Ugao;

    }

    glm::vec3 position() {
        return glm::vec3(lijevoDesno, goreDole, naprijedNazad);
    }

    td::Point<float> ugaoCijevi() {
        return { turretRotacija,barrelRotacija };
    }


    bool pucaj() {

        if (_crtajShell == false) {
            pucanjeAktivno = true;

            _crtajShell = true;

            _shellInitialx = lijevoDesno;

            _shellInitialy = goreDole;

            _shellInitialz = naprijedNazad;

            _shellInitialRotation1 = turretRotacija;

            _shellInitialRotation2 = barrelRotacija;

            _animShell = 0;

            animT = 2 * Vshell * sin(_shellInitialRotation2) / g;

            return true;
        }

        return false;
    }

    void postaviHeightMapu(std::vector<glm::vec3> &vertexi){
        _heightMap.postaviMapu(vertexi);
    }


    AABB getAABB() const {
        glm::vec3 halfSize = size * 0.5f;
        halfSize.y = 5.0f * 0.5f;

        std::vector<glm::vec3> corners = {
            {-halfSize.x, -halfSize.y, -halfSize.z},
            {-halfSize.x, -halfSize.y, +halfSize.z},
            {+halfSize.x, -halfSize.y, -halfSize.z},
            {+halfSize.x, -halfSize.y, +halfSize.z},
            {-halfSize.x, +halfSize.y, -halfSize.z},
            {-halfSize.x, +halfSize.y, +halfSize.z},
            {+halfSize.x, +halfSize.y, -halfSize.z},
            {+halfSize.x, +halfSize.y, +halfSize.z}
        };

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotacija, glm::vec3(0, 1, 0));

        glm::vec3 minAABB(FLT_MAX);
        glm::vec3 maxAABB(-FLT_MAX);

        for (const auto& corner : corners) {
            glm::vec3 rotatedCorner = glm::vec3(rotationMatrix * glm::vec4(corner, 1.0f)) + pozicijaURavni;
            minAABB = glm::min(minAABB, rotatedCorner);
            maxAABB = glm::max(maxAABB, rotatedCorner);
        }

        return { minAABB, maxAABB };
    }

    void takeDamage(int damage) {
        health -= damage; 
    }


    glm::vec3 dajPozicijuShella() {
        return pozicijaShella;
    }

    void pogodio() {
        pozicijaShella = glm::vec3{ 0.0f, -5.0f, 0.0f };
        _crtajShell = false;
    }

    std::vector<glm::vec3> getTankFootprintCorners() const {
        glm::vec3 halfSize = size * 0.5f;
        std::vector<glm::vec3> corners = {
            {-halfSize.x, 0, -halfSize.z},
            {-halfSize.x, 0, +halfSize.z},
            {+halfSize.x, 0, -halfSize.z},
            {+halfSize.x, 0, +halfSize.z}
        };

        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotacija, glm::vec3(0, 1, 0));

        for (auto& c : corners)
            c = glm::vec3(rot * glm::vec4(c, 1.0f)) + pozicijaURavni;

        return corners;
    }

    int getHealth() {
        return health;
    }


public:

    Tenk() :_textTank(":tankTex"), _textBarrel(":barrelTex"), _textTurret(":turretTex"), _textTracks(":tracksTex"), _textShell(":shellTex"),_textWheels(":wheelsTex"),_textRemains(":remainsTex")
    {
        naprijedNazad = 0;
        lijevoDesno = 0;
        turretRotacija = 0;
        barrelRotacija = 0;
        rotacija = 0;
        brzinaCijevi = 0;
        goreDole = 0;
    }
};