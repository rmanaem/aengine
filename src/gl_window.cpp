#include "gl_window.h"

GLWindow::GLWindow()
{
    width = 0;
    height = 0;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int GLWindow::initialize()
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

    window = glfwCreateWindow(width, height, "Triangle", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW window creation failed!!!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set the current context for GLEW to use
    glfwMakeContextCurrent(window);

    // Handle key = mouse input
    createCallbacks();

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed!!!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(window, this);
}

GLfloat GLWindow::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat GLWindow::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLWindow::createCallbacks()
{
    glfwSetKeyCallback(window, handleKeys);
    glfwSetCursorPosCallback(window, handleMouse);
}

void GLWindow::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    GLWindow *theWindow = static_cast<GLWindow *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void GLWindow::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
    GLWindow *theWindow = static_cast<GLWindow *>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}