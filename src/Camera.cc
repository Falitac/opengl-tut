#include "../include/Camera.hpp"

#include "../include/App.hpp"

Camera::Camera(App& app, const glm::vec3& initPosition)
    : app(app)
    , position(initPosition)
    , fieldOfView(48.f)
    , horizontalAngle(0.f)
    , verticalAngle(0.f)
{

}

void Camera::update(float dt)
{
  auto camSpeed = 0.f;
  auto strafeSpeed = 0.f;
  auto rotateHor = 0.0f;
  auto rotateVert = 0.0f;
  
  auto maxSpeed = 25.0f;
  if(app.keyMap[sf::Keyboard::LShift]) {
    maxSpeed /= 3.0f;
  }
  if(app.keyMap[sf::Keyboard::W]) {
    camSpeed += maxSpeed;
  }
  if(app.keyMap[sf::Keyboard::S]) {
    camSpeed -= maxSpeed;
  }
  if(app.keyMap[sf::Keyboard::A]) {
    strafeSpeed -= maxSpeed;
  }
  if(app.keyMap[sf::Keyboard::D]) {
    strafeSpeed += maxSpeed;
  }
  if(app.keyMap[sf::Keyboard::Q]) {
    position.y -= maxSpeed * dt;
  }
  if(app.keyMap[sf::Keyboard::E]) {
    position.y += maxSpeed * dt;
  }

  if(app.keyMap[sf::Keyboard::Space]) {
    auto& window = app.getWindow();
    auto deltaMouse = sf::Vector2i(window.getSize()) / 2 - sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window.getSize()) / 2, window);
    auto sensitivity = 2.2f;
    rotateHor = deltaMouse.x * sensitivity * dt;
    rotateVert = deltaMouse.y * sensitivity * dt;
  }

  camSpeed *= dt;
  strafeSpeed *= dt;
  horizontalAngle += glm::radians(rotateHor);
  verticalAngle += glm::radians(rotateVert);
  move(dir() * camSpeed + right() * strafeSpeed);
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

