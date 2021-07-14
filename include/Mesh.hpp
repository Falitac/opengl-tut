#pragma once

#include <glm/glm.hpp>
#include <gl/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <SFML/Graphics.hpp>


class Mesh {
public:
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
  };
  Mesh(const std::string& fileLocation);
  Mesh();
  ~Mesh();

  void loadFromFile(const std::string& fileLocation);
  void loadSphere(
    float radius,
    int verticalSubdivision,
    int horizontalSubdivisions
  );
  void update();
  void draw(sf::Shader& shader);

private:
  sf::Shader shader;
  std::map<std::string, sf::Texture> textures;
  std::string usedTexture;

  uint32_t vao;
  uint32_t vbo;
  uint32_t uvo;
  uint32_t nvo;

  void genBuffers();
  void passVerticesToBuffer();
  void deleteBuffers();

  void loadOBJ(std::ifstream& file);
  void loadMaterial(const std::string& path);
  std::vector<std::array<uint32_t, 3>> collectFaces(std::istringstream& lineStream);

  std::vector<Vertex> vertices;

};