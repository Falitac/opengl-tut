#pragma once

#include <glm/glm.hpp>
#include <gl/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <SFML/Graphics.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
};

class Mesh {
public:

  Mesh() = default;
  Mesh(const std::string& fileLocation);
  ~Mesh();

  void loadFromFile(const std::string& fileLocation);
  void update();
  void draw(sf::Shader& shader);

private:
  sf::Shader shader;
  std::map<int, sf::Texture> textures;

  uint32_t vao;
  uint32_t vbo;
  uint32_t uvo;
  uint32_t nvo;

  void genBuffers();
  void deleteBuffers();

  void loadOBJ(std::ifstream& file);
  std::vector<std::array<uint32_t, 3>> collectFaces(std::istringstream& lineStream);

  std::vector<Vertex> vertices;

};