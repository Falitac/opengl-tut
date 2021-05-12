#include "../include/App.hpp"

#include "../include/Sphere.hpp"
#include "../include/Icosphere.hpp"

App::App()
  : contextSettings(24, 8, 4, 4, 6)
  , titlePrefix("OpenGL ")
  , camera(*this, glm::vec3(0.f, 0.f, -8.f))
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
    std::cerr << "GLEW init failure" << std::endl;
    throw std::runtime_error("glew not supported");
  }

  mesh = std::make_unique<Mesh>("assets/objects/cylinder32.obj");

  glEnable(GL_DEPTH_BUFFER);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  if(!sf::Shader::isAvailable()) {
    std::printf("Shaders are not supported on this device\n");
  }
  worldCoordShader.loadFromFile("assets/shaders/coordv.glsl", "assets/shaders/coordf.glsl");

  mainLoop();
}

void App::handleEvents() {
  for(sf::Event event; window.pollEvent(event);) {
    switch(event.type) {
      case sf::Event::Closed:
        window.close();
      break;
      case sf::Event::KeyPressed: {
        keyMap[event.key.code] = true;
        if(event.key.control && keyMap[sf::Keyboard::W]) {
          window.close();
        }
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

void App::logic(float deltaTime) {
  camera.update(deltaTime);
}

void App::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  viewMatrix = glm::lookAt(
      camera.pos(),
      camera.pos() + camera.dir(),
      camera.up()
    );
  projectionView = projectionMatrix * viewMatrix;

  sf::Shader::bind(&worldCoordShader);
  worldCoordShader.setUniform("view", sf::Glsl::Mat4(&viewMatrix[0][0]));
  worldCoordShader.setUniform("projection", sf::Glsl::Mat4(&projectionMatrix[0][0]));
  constexpr auto pseudoNegInf = -10000.f;
  constexpr auto pseudoPosInf = +10000.f;
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

