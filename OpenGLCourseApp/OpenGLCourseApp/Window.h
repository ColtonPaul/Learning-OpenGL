#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    int initialize();
    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }

    bool* getKeys() { return keys; } //we could instead make the API request information on an individual key
    GLfloat getXChange();
    GLfloat getYChange();

    bool shouldClose() { return glfwWindowShouldClose(mainWindow); }
    void swapBuffers() { glfwSwapBuffers(mainWindow); };
    ~Window();
private:
    GLFWwindow * mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024]; //covers the range of keyboard characters to store key presses

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved; //takes into account the initial movement, so the screen doesn't jump when the mouse first moves into the screen

    void initializeKeys();

    //We have to put all five values in because it's expected for the API.
    //we need a static function to use it as a callback
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
    void createCallbacks();
};

