#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 initialPosition, glm::vec3 worldUp, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMovementSpeed, GLfloat initialTurnSpeed);
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    glm::mat4 calculateViewMatrix();

    ~Camera();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void update();
};

