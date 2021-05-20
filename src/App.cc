#include "../include/App.hpp"

#include "../include/Sphere.hpp"
#include "../include/Icosphere.hpp"

#include <filesystem>

App::App()
  : contextSettings(24, 8, 4, 4, 6)
  , titlePrefix("OpenGL ")
  , camera(*this, glm::vec3(0.f, 0.f, -8.f))
  , meshId(0)
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
  if(!sf::Shader::isAvailable()) {
    std::printf("Shaders are not supported on this device\n");
    throw std::runtime_error("shaders not supported");
  }
  glEnable(GL_DEPTH_BUFFER);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  namespace fs = std::filesystem;
  const fs::path objectsPath{"assets/objects/"};
  for(const auto& entry : fs::directory_iterator(objectsPath)) {
    const auto filename = entry.path().string();
    if(entry.is_regular_file() && entry.path().extension() == ".obj") {
      std::cout << "Found: " << filename << "\n";
      meshLocations.emplace_back(filename);
    }
  }
  meshLocations = {"assets/objects/nami-head.obj"};
  try {
    mesh = std::make_unique<Mesh>(meshLocations[meshId]);
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    std::exit(1.f);
  }


  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  shader.loadFromFile("assets/shaders/basic-v.glsl", "assets/shaders/basic-f.glsl");
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
        if(keyMap[sf::Keyboard::X]) {
          std::puts("reloading shader");
          shader.loadFromFile("assets/shaders/basic-v.glsl", "assets/shaders/basic-f.glsl");
        }
        bool reload = false;
        if(keyMap[sf::Keyboard::N]) {
          reload = true;
          if(meshId == 0) {
            meshId = meshLocations.size() - 1;
          } else {
            meshId--;
          }
        }
        if(keyMap[sf::Keyboard::M]) {
          reload = true;
          if(meshId == meshLocations.size() - 1) {
            meshId = 0;
          } else {
            meshId++;
          }
        }
        if(reload) {
          std::printf("Actual: %s\n", meshLocations[meshId].c_str());
          mesh = std::make_unique<Mesh>(meshLocations[meshId]);
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


  shader.setUniform("cameraDirection", sf::Glsl::Vec3(camera.dir().x, camera.dir().y, camera.dir().z));
  shader.setUniform("cameraPosition", sf::Glsl::Vec3(camera.pos().x, camera.pos().y, camera.pos().z));
  shader.setUniform("view", sf::Glsl::Mat4(&viewMatrix[0][0]));
  shader.setUniform("projection", sf::Glsl::Mat4(&projectionMatrix[0][0]));
  mesh->draw(shader);

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

