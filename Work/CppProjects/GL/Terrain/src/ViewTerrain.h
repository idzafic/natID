//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <glm/glm.hpp>
#include <rnd/Perlin.h>

//enable this definition to generate different (random) terrain on each app restart

//#define GEN_RANDOM

#define WIDTH 1024
#define HEIGHT 1024
#define H0 0.3f

#ifdef GEN_RANDOM
#define HIGHT_SCALE 100.f
#else
#define HIGHT_SCALE 200.f
#endif

class ViewTerrain : public gui::gl::View
{
    float _perlinNoise[WIDTH][HEIGHT];
    float _vertices[WIDTH * HEIGHT * 3];
    float _colors[WIDTH * HEIGHT * 3];
    unsigned int _indices[(WIDTH - 1) * (HEIGHT - 1) * 6];
    glm::mat4 _mvp;
    GLuint _vao, _vboVertices, _vboColors, _ebo;
    GLuint _shaderProgram;
    GLuint _locMVP;
    
private:
    GLuint compileShader(GLenum type, const char *src)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            mu::dbgLog("Shader error: %s", infoLog);
        }
        return shader;
    }
    
    GLuint linkShaderProgram()
    {
        const char *vertex_shader_src = "#version 410 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 vColor;\n"
            "uniform mat4 MVP;\n"
            "void main() {\n"
            "    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "    vColor = aColor;\n"
            "}";

        const char *fragment_shader_src = "#version 410 core\n"
            "in vec3 vColor;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = vec4(vColor, 1.0);\n"
            "}";
        
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader_src);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader_src);
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
    }
    
    //use Perlin's noise generator to generate random terrain with lakes, mountains (with some snow on top),...
    void generateTerrain()
    {
        double frequency = 3;
        std::int32_t octaves = 7;
#ifdef GEN_RANDOM
        std::uint32_t seed = std::uint32_t(time(nullptr)); //use current time as random seed
#else
        std::uint32_t seed = 7272266;
#endif

        //create and init Perlin noise generator
        const rnd::PerlinNoise<float, WIDTH> perlin(seed);
        
        const float fx = perlin.size() / frequency;
        const float fy = perlin.size() / frequency;
        
        int v = 0;
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                float h = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
                _perlinNoise[x][y] = h;

                _vertices[v * 3 + 0] = x;
                _vertices[v * 3 + 1] = h;
                _vertices[v * 3 + 2] = y;

                if (h < H0)
                {
                    //below sea level
                    float depth = (H0 - h) / H0;
                    _colors[v * 3 + 0] = 0.0f;
                    _colors[v * 3 + 1] = 0.4f * (1.0f - 2*depth); //darker blue for deeper areas
                    _colors[v * 3 + 2] = 0.8f * (1.0f - 2*depth) + 0.2f * depth; //darker blue for deeper areas
                    _vertices[v * 3 + 1] = 0; //set this to be sea level
                }
                else
                {
                    float delta = h - H0;
                    _vertices[v * 3 + 1] -= H0; //reduce to sea level
                    _vertices[v * 3 + 1] *= HIGHT_SCALE;
                    if (delta < 0.03f)
                    {
                        //sand color for areas close to zero ground level (beach)
                        _colors[v * 3 + 0] = 0.76f;
                        _colors[v * 3 + 1] = 0.70f;
                        _colors[v * 3 + 2] = 0.50f;
                    }
                    else if (delta < 0.3f)
                    {
                        _colors[v * 3 + 0] = 0.13f;
                        _colors[v * 3 + 1] = 0.55f-delta; //higher mountains -> darker green
                        _colors[v * 3 + 2] = 0.13f;
                    }
                    else if (delta < 0.4f)
                    {
                        _colors[v * 3 + 0] = 0.15f;
                        _colors[v * 3 + 1] = 0.52f-delta; //higher mountains -> darker green
                        _colors[v * 3 + 2] = 0.13f;
                    }
                    else
                    {
                        //snow
                        _colors[v * 3 + 0] = 1.0f;
                        _colors[v * 3 + 1] = 1.0f;
                        _colors[v * 3 + 2] = 1.0f;
                    }
                }
                v++;
            }
        }
        
        //triangulate - calculate triangle indices for EBO
        int i = 0;
        for (int y = 0; y < HEIGHT - 1; y++)
        {
            for (int x = 0; x < WIDTH - 1; x++)
            {
                int a = y * WIDTH + x;
                int b = a + 1;
                int c = a + WIDTH;
                int d = c + 1;

                _indices[i++] = a;
                _indices[i++] = c;
                _indices[i++] = b;
                _indices[i++] = b;
                _indices[i++] = c;
                _indices[i++] = d;
            }
        }
    }
    
    bool setupShaders()
    {
        _shaderProgram = linkShaderProgram();
        if (_shaderProgram < 0)
        {
            assert(false);
            return false;
        }
        //load MVP uniform location
        _locMVP = glGetUniformLocation(_shaderProgram, "MVP");
        
        glm::mat4 model(1.0f);
        //glm::mat4 view = glm::lookAt(glm::vec3(WIDTH / 2.0f, 60.0f, HEIGHT), glm::vec3(WIDTH / 2.0f, 0.0f, HEIGHT / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 view = glm::lookAt(glm::vec3(WIDTH / 2.0f, 600.0f, HEIGHT), glm::vec3(WIDTH / 2.0f, 0.0f, HEIGHT / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 1000.0f);
        _mvp = proj * view * model;
        
        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vboVertices);
        glGenBuffers(1, &_vboColors);
        glGenBuffers(1, &_ebo);

        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, _vboColors);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_colors), _colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        // Initialize OpenGL settings
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        
        generateTerrain();
        
        setupVBO();
        dbgCheckGLError();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(_shaderProgram);
        
        //set mvp matrix uniform
        glUniformMatrix4fv(_locMVP, 1, GL_FALSE, value_ptr(_mvp));

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, (WIDTH - 1) * (HEIGHT - 1) * 6, GL_UNSIGNED_INT, 0);
    }

public:
    ViewTerrain()
    {
        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewTerrain()
    {
        makeCurrentContext();
        // Delete OpenGL program
        if (_shaderProgram >= 0)
        {
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
        }
        // Delete OpenGL buffers
        if (_vao >= 0)
        {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
        if (_vboVertices >= 0)
        {
            glDeleteBuffers(1, &_vboVertices);
            _vboVertices = 0;
        }
        
        if (_vboColors >= 0)
        {
            glDeleteBuffers(1, &_vboColors);
            _vboColors = 0;
        }
        
        if (_ebo >= 0)
        {
            glDeleteBuffers(1, &_ebo);
            _ebo = 0;
        }
    }
};
