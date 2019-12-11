#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;
GLuint uniformModel;

bool direction = true; //true for right, false for left
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//TODO: These shaders should be defined in external files
//Vertex Shader
static const char* vShader = "                                              \n\
#version 330                                                                \n\
                                                                            \n\
layout (location = 0) in vec3 pos;                                          \n\
                                                                            \n\
uniform mat4 model;                                                         \n\
                                                                            \n\
out vec4 vCol; //vertex color                                               \n\
                                                                            \n\
void main()                                                                 \n\
{                                                                           \n\
     gl_Position = model * vec4(pos, 1.0);                                  \n\
     vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);   //make the color equal to whatever the position is                           \n\
}                                                                           \n\
";

//Fragment Shader
static const char* fShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 colour;                                                    \n\
                                                                    \n\
in vec4 vCol; //will get passed in from the vertex shader           \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
     colour = vCol; //rgb values                                    \n\
}                                                                   \n\
";

float toRadians(float input)
{
    const float pi = 3.14159265f;
    const float conversionFactor = pi / 180.0f;

    return input * conversionFactor;
}

void createTriangle()
{
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO); //we want to create 1 array and store it in the ID VAO
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); //create 1 buffer object (inside the bound VAO) and give it the ID VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer VBO
    glBindVertexArray(0); //unbind array VAO
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType); //creates an empty shader of the type shaderType

    const GLchar* theCode[1]; //an array of a single GLchar*
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength); //Will modify the actual source of the shader to equal the code we wrote
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 }; //a place to log the error

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); //TODO duplicated code
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: %s \n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader);
}

void compileShaders()
{
    shaderProgram = glCreateProgram(); //create the shader program and return the ID

    if (!shaderProgram)
    {
        printf("Error creating shader program");
        return;
    }

    AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 }; //a place to log the error

    glLinkProgram(shaderProgram); //creates the executables on the graphics card and links it together
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error linking program: %s \n", eLog);
        return;
    }

    glValidateProgram(shaderProgram); //checks if the shader is valid for the OpenGL context 
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s \n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderProgram, "model"); //will take the compiled shader and return the location of the var 'model'
}

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

    createTriangle();
    compileShaders();

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

        curAngle += 0.01f;
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
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model(1.0f);
        glm::vec3 zAxisVector(0.0f, 0.0f, 1.0f); //Here, only the direction matters, not the length
        //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
        //model = glm::rotate(model, toRadians(curAngle), zAxisVector);
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); //scale in the x axis and y axis by .4

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //binds the value of model to the model in the shader

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0); //unassign shader program

        //You have two scenes going on at once, one that can be seen, and one that you're drawing to
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}