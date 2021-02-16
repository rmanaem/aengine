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
#include "camera.h"

// Window dimensions
const GLint WIDTH = 1024, HEIGHT = 768;
// Convert to radian
const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Axes
std::vector<glm::vec3> axesVertices = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.1f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.1f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.1f),
    glm::vec3(0.0f, 0.0f, 1.0f),
};

GLuint axesVAO = 0, axesVBO = 0;
void createAxes()
{
    glGenVertexArrays(1, &axesVAO);
    glBindVertexArray(axesVAO);

    glGenBuffers(1, &axesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, axesVertices.size() * sizeof(glm::vec3), glm::value_ptr(axesVertices[0]), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void *)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Grid
std::vector<glm::vec3> gridVertices;

void createPoints(int n)
{
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            float x = (i - n / 2) / (n / 2.0);
            float z = (j - n / 2) / (n / 2.0);
            gridVertices.push_back(glm::vec3(x, 0.0f, z));
            gridVertices.push_back(glm::vec3(0.662f, 0.662f, 0.662f));
            gridVertices.push_back(glm::vec3(0.0f, 0.0f, z));
            gridVertices.push_back(glm::vec3(0.662f, 0.662f, 0.662f));
        }
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            float x = (i - n / 2) / (n / 2.0);
            float z = (j - n / 2) / (n / 2.0);
            gridVertices.push_back(glm::vec3(x, 0.0f, z));
            gridVertices.push_back(glm::vec3(0.662f, 0.662f, 0.662f));
            gridVertices.push_back(glm::vec3(x, 0.0f, -z));
            gridVertices.push_back(glm::vec3(0.662f, 0.662f, 0.662f));
        }
    }
}

GLuint gridVAO = 0, gridVBO = 0;
void createGrid()
{
    glGenVertexArrays(1, &gridVAO);
    glBindVertexArray(gridVAO);

    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(glm::vec3), glm::value_ptr(gridVertices[0]), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void *)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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
const char *vertexShader = "./src/shaders/vertex1";

// Fragment Shader
const char *fragmentShader = "./src/shaders/fragment1";

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
    createPoints(128);

    camera = Camera(glm::vec3(0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.0005f, 0.1f);

    GLuint uniformModel = 0,
           uniformProjection = 0, uniformView = 0;

    // Proejction matrix
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {

        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime - now;

        // Get and handle user input events
        glfwPollEvents();

        // Check for key and mouse presses
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();

        // Model matrix
        glm::mat4 model(1.0f);

        // Updating the uniform variable
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        createAxes();
        // Render Axes
        model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(axesVAO);
        glDrawArrays(GL_LINES, 0, axesVertices.size());
        glBindVertexArray(0);

        createGrid();
        // Render grid
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size());
        glBindVertexArray(0);

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}