#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    this->clearShader();
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
    compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char * vertexLocation, const char * fragmentLocation)
{
    std::string vertexCodeString = this->readFile(vertexLocation);
    std::string fragmentCodeString = this->readFile(fragmentLocation);

    const char* vertexCode = vertexCodeString.c_str();
    const char* fragmentCode = fragmentCodeString.c_str();

    compileShader(vertexCode, fragmentCode);
}

GLuint Shader::getProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::getModelLocation()
{
    return uniformModel;
}

GLuint Shader::getViewLocation()
{
    return uniformView;
}

GLuint Shader::getAmbientIntensityLocation()
{
    return uniformAmbientIntensity;
}

GLuint Shader::getAmbientColorLocation()
{
    return uniformAmbientColor;
}

void Shader::useShader()
{
    glUseProgram(shaderID);
}

void Shader::clearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::compileShader(const char * vertexCode, const char * fragmentCode)
{
    shaderID = glCreateProgram(); //create the shader program and return the ID

    if (!shaderID)
    {
        printf("Error creating shader program \n");
        return;
    }

    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 }; //a place to log the error

    glLinkProgram(shaderID); //creates the executables on the graphics card and links it together
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: %s \n", eLog);
        return;
    }

    glValidateProgram(shaderID); //checks if the shader is valid for the OpenGL context 
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s \n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model"); //will take the compiled shader and return the location of the var 'model'
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.color");
    uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
}

void Shader::addShader(GLuint program, const char * shaderCode, GLenum shaderType)
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

std::string Shader::readFile(const char * fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s! File doesn't exist \n", fileLocation);
        return "";
    }
    
    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
