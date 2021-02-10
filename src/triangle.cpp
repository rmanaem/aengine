#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh *> meshList;

GLuint shader, uniformModel, uniformProjection;

// Directions for moving the triangle
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float rotationAngle = 0.0f;

bool sizeDirection = true;
float size = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Create VAO and VBO
void createVertexArrayObject()
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
const char *vertexShader()
{
    return "#version 330\n"
           "layout (location = 0) in vec3 pos;"
           "out vec4 vertexCol;"
           "uniform mat4 model;"
           "uniform mat4 projection;"
           "void main()"
           "{"
           "    gl_Position = projection * model * vec4(pos, 1.0);"
           "    vertexCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);"
           "}";
}

// Fragment Shader
const char *fragmentShader()
{
    return "#version 330\n"
           "out vec4 color;"
           "in vec4 vertexCol;"
           "void main()"
           "{"
           "    color = vertexCol;"
           "}";
}

void addShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the shader program: \n"
                  << shaderType << eLog << std::endl;
        return;
    }

    glAttachShader(program, theShader);
}

void compileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        std::cout << "Error creating shader program!" << std::endl;
        return;
    }

    addShader(shader, vertexShader(), GL_VERTEX_SHADER);
    addShader(shader, fragmentShader(), GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: \n"
                  << eLog << std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: \n"
                  << eLog << std::endl;
        return;
    }

    // Grab the location of uniform variable
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Set up GLFW window properties
    // openGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", NULL, NULL);
    if (!mainWindow)
    {
        std::cout << "GLFW window creation failed!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed!!!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    createVertexArrayObject();
    compileShaders();

    // Proejction matrix
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }

        rotationAngle += 0.0001f;
        if (rotationAngle >= 360)
        {
            rotationAngle -= 360;
        }

        if (sizeDirection)
        {
            size += 0.0001f;
        }
        else
        {
            size -= 0.0001f;
        }

        if (size >= maxSize || size <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing the triangle
        glUseProgram(shader);

        // Model matrix
        glm::mat4 model(1.0f);

        // Translation
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
        // Rotation
        // model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        // Scale
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        // Updating the uniform variable to transform the triangle
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->renderMesh();

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}