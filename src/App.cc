#include "../include/App.hpp"

#include "../include/Sphere.hpp"
#include "../include/Icosphere.hpp"

App::App()
  : contextSettings(24, 8, 4, 4, 6)
  , titlePrefix("OpenGL ")
  , camera(glm::vec3(0.f, 0.f, -8.f))
  , triangleNum(1)
{
}

void App::run() {
  window.create(
    {1600, 900},
    titlePrefix,
    sf::Style::Default,
    contextSettings
  );

  window.setActive(true);

  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    std::cout << "GLEW init failure" << std::endl;
  }

  generators::generateIcosphere(vertices, indices, 1.0, 0);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);


  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glEnable(GL_DEPTH_BUFFER);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  shaderID = LoadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

  mainLoop();
}

void App::handleEvents() {
  for(sf::Event event; window.pollEvent(event);) {
    switch(event.type) {
      case sf::Event::Closed:
        window.close();
      break;
      case sf::Event::KeyPressed:
        keyMap[event.key.code] = true;
        if(event.key.control && keyMap[sf::Keyboard::W]) {
          window.close();
        }
        if(keyMap[sf::Keyboard::O]) {
          triangleNum += 1;
        }
        if(keyMap[sf::Keyboard::P]) {
          triangleNum -= 1;
        }
      break;
      case sf::Event::KeyReleased:
        keyMap[event.key.code] = false;
      break;
      case sf::Event::Resized:
        handleResize();
      default:
      break;
    }
  }

}

void App::logic(const float& deltaTime) {
  auto camSpeed = 0.f;
  auto strafeSpeed = 0.f;
  auto rotateHor = 0.0f;
  auto rotateVert = 0.0f;
  
  auto maxSpeed = 25.0f;
  if(keyMap[sf::Keyboard::LShift]) {
    maxSpeed /= 3.0f;
  }
  if(keyMap[sf::Keyboard::W]) {
    camSpeed += maxSpeed;
  }
  if(keyMap[sf::Keyboard::S]) {
    camSpeed -= maxSpeed;
  }
  if(keyMap[sf::Keyboard::A]) {
    strafeSpeed -= maxSpeed;
  }
  if(keyMap[sf::Keyboard::D]) {
    strafeSpeed += maxSpeed;
  }
  if(keyMap[sf::Keyboard::Q]) {
    camera.pos().y -= maxSpeed * deltaTime;
  }
  if(keyMap[sf::Keyboard::E]) {
    camera.pos().y += maxSpeed * deltaTime;
  }


  if(keyMap[sf::Keyboard::X]) {
    glDeleteProgram(shaderID);
    shaderID = LoadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
  }

  if(keyMap[sf::Keyboard::Space]) {
    auto deltaMouse = sf::Vector2i(window.getSize()) / 2 - sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window.getSize()) / 2, window);
    auto sensitivity = 2.2f;
    rotateHor = deltaMouse.x * sensitivity * deltaTime;
    rotateVert = deltaMouse.y * sensitivity * deltaTime;
  }

  camSpeed *= deltaTime;
  strafeSpeed *= deltaTime;
  camera.horAngle() += glm::radians(rotateHor);
  camera.vertAngle() += glm::radians(rotateVert);
  camera.move(camera.dir() * camSpeed + camera.right() * strafeSpeed);
}

void App::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera.update();
  viewMatrix = glm::lookAt(
      camera.pos(),
      camera.pos() + camera.dir(),
      camera.up()
    );
  projectionView = projectionMatrix * viewMatrix;


  glUseProgram(shaderID);
  GLint viewID = glGetUniformLocation(shaderID, "view");
  GLint projectionID = glGetUniformLocation(shaderID, "projection");
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);
  
  glBindVertexArray(vao);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLES, 3*triangleNum, GL_UNSIGNED_INT, 0);

  constexpr auto pseudoNegInf = -1000000.f;
  constexpr auto pseudoPosInf = +1000000.f;
  glBegin(GL_LINES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(pseudoPosInf , 0.f, 0.f);
    glVertex3f(pseudoNegInf ,  0.f, 0.f);
  glEnd();
  glBegin(GL_LINES);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, pseudoNegInf, 0.f);
    glVertex3f(0.f,  pseudoPosInf, 0.f);
  glEnd();
  glBegin(GL_LINES);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.f, pseudoNegInf);
    glVertex3f(0.f, 0.f,  pseudoPosInf);
  glEnd();


  glBindVertexArray(0);
  window.pushGLStates();
  window.popGLStates();
  window.display();
}

void App::handleResize() {
  glViewport(0, 0, window.getSize().x, window.getSize().y);
  projectionMatrix = glm::perspective(
      glm::radians(45.0f),
      getAspectRatio(),
      0.1f,
      300.0f
    );
}

void App::mainLoop() {
  sf::Time tickTime = sf::seconds(1.0f / static_cast<float>(tickCount));
  sf::Time timeAccumulator = sf::seconds(0.0f);
  sf::Time lastFrameTime   = sf::seconds(0.0f);
  sf::Clock timer;

  startClock.restart();
  timer.restart();
  fpsClock.restart();
  handleResize();

  while(window.isOpen()) {
    sf::Time timeDifference = timer.getElapsedTime() - lastFrameTime;
    timeAccumulator += timeDifference;
    lastFrameTime = timer.getElapsedTime();

    handleEvents();

    while(timeAccumulator >= tickTime) {
      logic(tickTime.asSeconds());
      timeAccumulator -= tickTime;
      #ifndef NDEBUG
        actualTickCount++;
      #endif
    }

    draw();

    #ifndef NDEBUG
      showDebugInfo();
    #endif
  }
}

void App::showDebugInfo() {
  fpsCount++;
  if(fpsClock.getElapsedTime() > sf::seconds(1.0f)) {
    window.setTitle(
        titlePrefix
      + "FPS: "
      + std::to_string(fpsCount)
      + " TICKS: "
      + std::to_string(actualTickCount)
    );
    fpsClock.restart();
    fpsCount = 0;
    actualTickCount = 0;
  }
}

