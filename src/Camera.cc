#include "../include/Camera.hpp"

Camera::Camera(const glm::vec3& initPosition):
position(initPosition),
fieldOfView(48.f),
horizontalAngle(0.f),
verticalAngle(0.f)
{

}

void Camera::update()
{
    direction = glm::vec3(
        std::cos(verticalAngle) * std::sin(horizontalAngle),
        std::sin(verticalAngle),
        std::cos(verticalAngle) * std::cos(horizontalAngle)
    );
    rightDirection = glm::vec3(
        sin(horizontalAngle - glm::half_pi<float>()),
        0,
        cos(horizontalAngle - glm::half_pi<float>())
    );
    upDirection = glm::cross(rightDirection, direction);
}

