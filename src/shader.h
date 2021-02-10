#pragma once

#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void createFromString(const char *vertexCode, const char *fragmentCode);

    GLuint getModelLocation();
    GLuint getProjectionLocation();

    void useShader();
    void clearShader();

    ~Shader();

private:
    GLuint shaderID, uniformModel, uniformProjection;

    void addShader(GLuint program, const char *shaderCode, GLenum shaderType);
    void compileShader(const char *vertexCode, const char *fragmentCode);
};