#include "Camera.h"

Camera::Camera()
    : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f)
{
}

Camera::Camera(glm::vec3 initialPosition, glm::vec3 worldUp, GLfloat initialYaw, GLfloat initialPitch, GLfloat initialMovementSpeed, GLfloat initialTurnSpeed)
{
    position = initialPosition;
    this->worldUp = worldUp;
    yaw = initialYaw;
    pitch = initialPitch;
    front = glm::vec3(0.0, 0.0f, -1.0f);

    movementSpeed = initialMovementSpeed;
    turnSpeed = initialTurnSpeed;

    this->update();
}

void Camera::keyControl(bool * keys, GLfloat deltaTime)
{
    GLfloat velocity = movementSpeed * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;
    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f) //TODO-CJP: research why we need this restriction
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    this->update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

Camera::~Camera()
{
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front); //We want normalized vectors so that movement calculations will not be affected by meaningless magnitude

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
