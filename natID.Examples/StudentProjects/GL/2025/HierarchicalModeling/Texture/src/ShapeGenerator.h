#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Pomo?na struktura za vra?anje podataka iz funkcija
struct ShapeData {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
};

namespace ShapeGenerator
{
    // Generiše podatke za kocku dimenzija 1x1x1 sa centrom u (0,0,0)
    ShapeData createCube()
    {
        ShapeData data;
        data.vertices = {
            // Prednja strana
            {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
            // Zadnja strana
            {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
            // Lijeva strana
            {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
            // Desna strana
            { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
            // Gornja strana
            {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            // Donja strana
            {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}
        };

        data.indices = {
            0, 1, 2, 0, 2, 3,       // Prednja
            4, 5, 6, 4, 6, 7,       // Zadnja
            8, 9, 10, 8, 10, 11,    // Lijeva
            12, 13, 14, 12, 14, 15, // Desna
            16, 17, 18, 16, 18, 19, // Gornja
            20, 21, 22, 20, 22, 23  // Donja
        };

        // Normale su jednostavne za kocku
        data.normals = {
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
            {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
        };

        data.uvs = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
            {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f},
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        return data;
    }

    // Generiše podatke za valjak (cilindar)
    // height - visina, radius - polupre?nik, sectors - broj segmenata (ivica) kruga
    ShapeData createCylinder(float height, float radius, int sectors)
    {
        ShapeData data;
        const float PI = glm::pi<float>();
        float sectorStep = 2.0f * PI / sectors;
        float sectorAngle;

        // ----------- 1. OMOTAČ (plašt) -----------
        for (int i = 0; i <= sectors; ++i)
        {
            sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);
            data.vertices.push_back(glm::vec3(x * radius, height * 0.5f, z * radius));
            data.vertices.push_back(glm::vec3(x * radius, -height * 0.5f, z * radius));
            data.normals.push_back(glm::vec3(x, 0, z));
            data.normals.push_back(glm::vec3(x, 0, z));
            data.uvs.push_back(glm::vec2((float)i / sectors, 1.0f));
            data.uvs.push_back(glm::vec2((float)i / sectors, 0.0f));
        }

        for (int i = 0; i < sectors; ++i)
        {
            int i1 = i * 2;
            int i2 = i1 + 1;
            int i3 = i1 + 2;
            int i4 = i1 + 3;
            data.indices.push_back(i1); data.indices.push_back(i2); data.indices.push_back(i3);
            data.indices.push_back(i3); data.indices.push_back(i2); data.indices.push_back(i4);
        }

        // ----------- 2. BAZE (GORNJA I DONJA) -----------

        // GORNJA BAZA
        int topCenterIndex = (int)data.vertices.size();
        data.vertices.push_back(glm::vec3(0.0f, height * 0.5f, 0.0f));
        data.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        data.uvs.push_back(glm::vec2(0.5f, 0.5f));

        int firstVertexOfTopBase = topCenterIndex + 1;
        for (int i = 0; i <= sectors; ++i)
        {
            sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);
            data.vertices.push_back(glm::vec3(x * radius, height * 0.5f, z * radius));
            data.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            data.uvs.push_back(glm::vec2(x * 0.5f + 0.5f, z * 0.5f + 0.5f));
        }

        for (int i = 0; i < sectors; ++i)
        {
            data.indices.push_back(topCenterIndex);
            data.indices.push_back(firstVertexOfTopBase + i);
            data.indices.push_back(firstVertexOfTopBase + i + 1);
        }

        // DONJA BAZA
        int bottomCenterIndex = (int)data.vertices.size();
        data.vertices.push_back(glm::vec3(0.0f, -height * 0.5f, 0.0f));
        data.normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        data.uvs.push_back(glm::vec2(0.5f, 0.5f));

        int firstVertexOfBottomBase = bottomCenterIndex + 1;
        for (int i = 0; i <= sectors; ++i)
        {
            sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);
            data.vertices.push_back(glm::vec3(x * radius, -height * 0.5f, z * radius));
            data.normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            data.uvs.push_back(glm::vec2(x * 0.5f + 0.5f, z * 0.5f + 0.5f));
        }

        for (int i = 0; i < sectors; ++i)
        {
            // ISPRAVLJEN REDOSLIJED ZA CULLING
            data.indices.push_back(bottomCenterIndex);
            data.indices.push_back(firstVertexOfBottomBase + i + 1);
            data.indices.push_back(firstVertexOfBottomBase + i);
        }

        return data;
    }


    ShapeData createSphere(float radius, int sectors, int stacks)
    {
        ShapeData data;

        const float PI = glm::pi<float>();
        float sectorStep = 2.0f * PI / sectors;
        float stackStep = PI / stacks;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = PI / 2.0f - i * stackStep; // Ugao od gore (PI/2) ka dole (-PI/2)
            float xy = radius * cos(stackAngle);      // Poluprečnik prstena na trenutnoj visini
            float z = radius * sin(stackAngle);       // Z koordinata (visina) na prstenu

            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep; // Ugao oko Z ose

                // Pozicija vrha (vertex)
                float x = xy * cos(sectorAngle);
                float y = xy * sin(sectorAngle);
                data.vertices.push_back(glm::vec3(x, y, z));

                // Normala (za sferu, normala je ista kao i normalizovani vektor od centra do vrha)
                data.normals.push_back(glm::normalize(glm::vec3(x, y, z)));

                // UV koordinate
                float u = (float)j / sectors;
                float v = (float)i / stacks;
                data.uvs.push_back(glm::vec2(u, v));
            }
        }

        // Generisanje indeksa za trouglove
        // Iscrtavamo četverouglove između svaka 4 susjedna vrha
        for (int i = 0; i < stacks; ++i)
        {
            int k1 = i * (sectors + 1); // Početak trenutnog prstena
            int k2 = k1 + sectors + 1;  // Početak sljedećeg prstena

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
            {
                // Dva trougla koja čine četverougao
                if (i != 0)
                {
                    data.indices.push_back(k1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1))
                {
                    data.indices.push_back(k1 + 1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k2 + 1);
                }
            }
        }

        return data;
    }

    // U ShapeGenerator.h, unutar namespace-a

    // Generiše podatke za polusferu (hemisferu)
    ShapeData createHemisphere(float radius, int sectors, int stacks)
    {
        ShapeData data;

        const float PI = glm::pi<float>();
        float sectorStep = 2.0f * PI / sectors;
        float stackStep = (PI / 2.0f) / stacks; // **KLJUČNA IZMJENA**: Idemo samo do pola (90 stepeni)
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = PI / 2.0f - i * stackStep; // Ugao ide od gore (PI/2) do ekvatora (0)
            float xy = radius * cos(stackAngle);
            float z = radius * sin(stackAngle);

            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep;

                float x = xy * cos(sectorAngle);
                float y = xy * sin(sectorAngle);
                data.vertices.push_back(glm::vec3(x, y, z));
                data.normals.push_back(glm::normalize(glm::vec3(x, y, z)));

                float u = (float)j / sectors;
                float v = (float)i / stacks;
                data.uvs.push_back(glm::vec2(u, v));
            }
        }

        // Generisanje indeksa ostaje skoro isto
        for (int i = 0; i < stacks; ++i)
        {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
            {
                data.indices.push_back(k1);
                data.indices.push_back(k2);
                data.indices.push_back(k1 + 1);

                data.indices.push_back(k1 + 1);
                data.indices.push_back(k2);
                data.indices.push_back(k2 + 1);
            }
        }

        // NOVO: Zatvaranje baze polusfere
        int baseCenterIndex = (int)data.vertices.size();
        data.vertices.push_back(glm::vec3(0, 0, 0)); // Centar baze
        data.normals.push_back(glm::vec3(0, 0, -1.0f)); // Normala gleda dole
        data.uvs.push_back(glm::vec2(0.5f, 0.5f));

        int firstVertexOfBase = stacks * (sectors + 1);
        for (int i = 0; i < sectors; ++i)
        {
            data.indices.push_back(baseCenterIndex);
            data.indices.push_back(firstVertexOfBase + i + 1);
            data.indices.push_back(firstVertexOfBase + i);
        }

        return data;
    }
}


