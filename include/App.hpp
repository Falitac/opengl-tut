#pragma once

#include "Camera.hpp"
#include "Mesh.hpp"
#include "LoadShader.hpp"
#include "Random.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include <limits>
#include <iostream>


class App
{
public:
    App();
    ~App() = default;

    void run();

    template<typename T = float>
    inline T getAspectRatio() {
        return static_cast<T>(window.getSize().x) / window.getSize().y;
    }
    const sf::RenderWindow& getWindow() const {
        return window;
    }
public:
    std::map<int, bool> keyMap;

private:
    void mainLoop();
    void handleEvents();
    void logic(float deltaTime);
    void draw();
    void handleResize();

private:
    std::string titlePrefix;
    sf::RenderWindow window;
    sf::ContextSettings contextSettings;

    std::unique_ptr<Mesh> mesh;
    std::vector<std::string> meshLocations;
    std::size_t meshId;

    sf::Shader worldCoordShader;
    sf::Shader shader;

    // MVP matrix
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionView;

    Camera camera;

    sf::Clock startClock;

    // Performance data
    sf::Clock fpsClock;
    unsigned fpsCount = 0;
    unsigned actualTickCount = 0;
    const unsigned tickCount = 120;

    int divisions = 4;

    void showDebugInfo();
};
