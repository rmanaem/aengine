#pragma once

#include <string>
#include <Cstring>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void createFromString(const char *vertexCode, const char *fragmentCode);
    void createFromFiles(const char *vertexLocation, const char *fragmentLocation);

    std::string ReadFile(const char *fileLocation);

    GLuint getModelLocation() { return uniformModel; }
    GLuint getProjectionLocation() { return uniformProjection; }
    GLuint getViewLocation() { return uniformView; }

    void useShader();
    void clearShader();

    ~Shader();

private:
    GLuint shaderID, uniformModel, uniformProjection, uniformView;

    void addShader(GLuint program, const char *shaderCode, GLenum shaderType);
    void compileShader(const char *vertexCode, const char *fragmentCode);
};