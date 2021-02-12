#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
public:
    GLWindow();
    GLWindow(GLint windowWidth, GLint windowHeight);

    int initialize();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(window); }

    bool *getKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { return glfwSwapBuffers(window); }

    ~GLWindow();

private:
    GLFWwindow *window;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    void createCallbacks();
    static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};