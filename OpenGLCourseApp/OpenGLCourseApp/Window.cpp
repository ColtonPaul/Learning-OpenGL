#include "Window.h"

#include <cstdlib>

Window::Window()
{
    width = 800;
    height = 600;
    this->initializeKeys();
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    this->initializeKeys();
}

void Window::initializeKeys()
{
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

void Window::handleKeys(GLFWwindow * window, int key, int code, int action, int mode)
{
    //This gets the window that we set in Window::initialize(), which lets us work with the window in a static 
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

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

void Window::handleMouse(GLFWwindow * window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseFirstMoved) //if it's the very first movement
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

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys); //Like a signal/slot between a key press on the main window and the callback handleKeys;
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

int Window::initialize()
{
    //initialize GLFW
    if (!glfwInit())
    {
        //handle init failing
        printf("GLFW init failed\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //Set up GLFW window properties
    //OpenGL version -- set to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Don't allow deprecated features
    //Do make it forward compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create the window
    mainWindow = glfwCreateWindow(width, height, "Spinning 3D Tetrahedron", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for GLEW to use
    //I.e. We want to tie the openGL context to this window
    glfwMakeContextCurrent(mainWindow);

    //Handle key and mouse input
    this->createCallbacks();

    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Doesn't display the cursor to the screen

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        printf("Glew init failed\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST); //enables depth testing to determine which triangles are deeper in the image -- which triangles should be drawn on top

    //Set up viewport size (Sets up what part we're drawing to on our window)
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
