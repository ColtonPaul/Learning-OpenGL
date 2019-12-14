#include "Light.h"



Light::Light()
    : color(glm::vec3(1.0f, 1.0f, 1.0f))
    , ambientIntensity(1.0)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity)
    : color(glm::vec3(red, green, blue))
    , ambientIntensity(ambientIntensity)
{
}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation) //why are these floats if they're just locations??
{
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity); //feeds the ambient intensity value to the ambientIntensity location in the shader
}


Light::~Light()
{
}
