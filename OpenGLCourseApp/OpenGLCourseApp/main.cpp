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

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
    mainWindow = Window(800, 600);
    mainWindow.initialize();
    createObjects();
    createShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(mainWindow.getBufferWidth() / mainWindow.getBufferHeight()), 0.1f, 100.0f);

    while (!mainWindow.shouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        //Handle user input events
        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        //Clears window
        glClearColor(0.0, 0.0, 0.0, 1); //passed rgb values between 0 and 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears both the color buffer bit and the depth buffer bit

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();
        glm::mat4 model(1.0f);
        glm::vec3 zAxisVector(0.0f, 0.0f, 1.0f); //Here, only the direction matters, not the length
        glm::vec3 yAxisVector(0.0f, 1.0f, 0.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale in the x axis and y axis by .4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //binds the value of model to the model in the shader
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale in the x axis and y axis by .4
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //binds the value of model to the model in the shader
        meshList[1]->renderMesh();


        glUseProgram(0); //unassign shader program
        //You have two scenes going on at once, one that can be seen, and one that you're drawing to
        mainWindow.swapBuffers();
    }

    return 0;
}