#pragma once

#include <GL/glew.h>

class Mesh
{
public:
    Mesh();

    void createMesh(GLfloat *vertices, unsigned *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void renderMesh();
    void clearMesh();
    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei numOfIndices;
};