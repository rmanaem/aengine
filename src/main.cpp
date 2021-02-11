#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl_window.h"
#include "mesh.h"
#include "shader.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
// Convert to radian
const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// Create VAO, VBO, and IBO
void createObjects()
{

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};

    GLfloat vertices[] = {
        -1.0f, -1.0, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    Mesh *obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

// Vertex Shader
const char *vertexShader = "./src/shaders/vertex";

// Fragment Shader
const char *fragmentShader = "./src/shaders/fragment";

void createShaders()
{
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vertexShader, fragmentShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = GLWindow(WIDTH, HEIGHT);
    mainWindow.initialize();

    createObjects();
    createShaders();

    GLuint uniformModel = 0, uniformProjection = 0;

    // Proejction matrix
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        // Get and handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing the triangle
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();

        // Model matrix
        glm::mat4 model(1.0f);

        // Translation
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // Rotation
        // model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        // Scale
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        // Updating the uniform variable to transform the triangle
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->renderMesh();

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}