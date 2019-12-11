#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

bool direction = true; //true for right, false for left
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//TODO: These shaders should be defined in external files
//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

float toRadians(float input)
{
    const float pi = 3.14159265f;
    const float conversionFactor = pi / 180.0f;

    return input * conversionFactor;
}

void createObjects()
{

    unsigned int indices[] =
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh* tetrahedron = new Mesh();
    tetrahedron->createMesh(vertices, indices, 12, 12);
    meshList.push_back(tetrahedron);

    Mesh* secondTetrahedron = new Mesh();
    secondTetrahedron->createMesh(vertices, indices, 12, 12);
    meshList.push_back(secondTetrahedron);
}

void createShaders()
{
    Shader * shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
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
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Spinning 3D Tetrahedron", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed\n");
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
        printf("Glew init failed\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST); //enables depth testing to determine which triangles are deeper in the image -- which triangles should be drawn on top

    //Set up viewport size (Sets up what part we're drawing to on our window)
    glViewport(0, 0, bufferWidth, bufferHeight); //why buffer dims instead of WIDTH, HEIGHT?

    createObjects();
    createShaders();

    GLuint uniformProjection = 0, uniformModel = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(mainWindow))
    {
        //Handle user input events
        glfwPollEvents(); //checks for any user events

        if (direction) //if heading to the right
        {
            triOffset += triIncrement;
        }
        else //if heading to the left
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }

        curAngle += 0.1f;
        if (curAngle >= 360)
        {
            curAngle -= 360; //not really necessary, but we don't want an overflow.
        }

        if (sizeDirection)
        {
            curSize += 0.0001f;
        }
        else
        {
            curSize -= 0.0001f;
        }
        if (curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        //Clears window
        glClearColor(0.0, 0.0, 0.0, 1); //passed rgb values between 0 and 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears both the color buffer bit and the depth buffer bit

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        glm::mat4 model(1.0f);
        glm::vec3 zAxisVector(0.0f, 0.0f, 1.0f); //Here, only the direction matters, not the length
        glm::vec3 yAxisVector(0.0f, 1.0f, 0.0f);
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale in the x axis and y axis by .4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //binds the value of model to the model in the shader
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //binds the value of model to the model in the shader
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale in the x axis and y axis by .4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //binds the value of model to the model in the shader
        meshList[1]->renderMesh();


        glUseProgram(0); //unassign shader program
        //You have two scenes going on at once, one that can be seen, and one that you're drawing to
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}