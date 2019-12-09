#include <stdio.h>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main()
{
    //initialize GLFW
    if (!glfwInit())
    {
        //handle init failing
        printf("GLFW init failed");
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
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "My Window Title", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for GLEW to use
    //I.e. We want to tie the openGL context to this window
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        printf("Glew init failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //Set up viewport size (Sets up what part we're drawing to on our window)
    glViewport(0, 0, bufferWidth, bufferHeight); //why buffer dims instead of WIDTH, HEIGHT?

    while (!glfwWindowShouldClose(mainWindow))
    {
        //Handle user input events
        glfwPollEvents(); //checks for any user events

        //Clears window
        glClearColor(3.0f/256, 252.0f/256, 252.0f/256, 1); //passed rgb values between 0 and 1
        glClear(GL_COLOR_BUFFER_BIT);

        //You have two scenes going on at once, one that can be seen, and one that you're drawing to
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}