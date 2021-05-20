#include "../include/Mesh.hpp"

#include <filesystem>
#include <optional>
#include <iostream>


Mesh::Mesh(const std::string& fileLocation) {
  genBuffers();
  loadFromFile(fileLocation);
}

Mesh::~Mesh() {
  deleteBuffers();
}

void Mesh::loadFromFile(const std::string& fileLocation) {
  std::ifstream inputFile(fileLocation);
  auto fileExtension = std::filesystem::path(fileLocation).extension();
  if(inputFile.is_open()) {
    if(fileExtension == ".obj") {
      std::cout << "It's an obj!\n";
      loadOBJ(inputFile);
      glBindVertexArray(vao);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
      glEnableVertexAttribArray(1);

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

      glBindVertexArray(0);
      std::puts("done!");
    }
  }
}

void Mesh::update() {

}

void Mesh::draw(sf::Shader& shader) {
  auto& currentTexture = textures[usedTexture];
  glBindVertexArray(vao);
  sf::Shader::bind(&shader);
  shader.setUniform("texture1", currentTexture);
  sf::Texture::bind(&currentTexture);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
}

void Mesh::genBuffers() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
}

void Mesh::deleteBuffers() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Mesh::loadOBJ(std::ifstream& file) {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  std::vector<std::array<uint32_t, 3>> indices;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream lineStream(line);
    std::string lineStart;
    lineStream >> lineStart;

    if(lineStart == "v") {
      float x, y, z;
      lineStream >> x >> y >> z;
      vertices.emplace_back(x, y, z);
    } else if (lineStart == "vn") {
      float x, y, z;
      lineStream >> x >> y >> z;
      normals.emplace_back(x, y, z);
    } else if (lineStart == "vt") {
      float x, y, z;
      lineStream >> x >> y;
      uvs.emplace_back(x, y);
    } else if (lineStart == "f") {
      uint32_t index;
      auto faces = collectFaces(lineStream);
      if(faces.size() >= 3) {
        auto first = faces.front();
        for(auto it = faces.begin() + 2; it != faces.end(); it++) {
          indices.push_back(first);
          indices.push_back(*(it - 1));
          indices.push_back(*it);
        }
      }
    } else if (lineStart == "o") {
      std::string objectName;
      lineStream >> objectName;
      std::printf("Object name %s\n", objectName.c_str());
    } else if (lineStart == "mtllib") {
      std::string materialPath;
      lineStream >> materialPath;
      loadMaterial("assets/objects/" + materialPath);
    } else if (lineStart == "usemtl") {
      lineStream >> usedTexture;
    } else if (lineStart == "s") {
      std::string smoothShading;
      lineStream >> smoothShading;
    } else if (lineStart == "#") {
    } else {
      // skip line
    }
  }
  std::printf("reading done!\n");
  if(vertices.empty()) {
    throw std::runtime_error("no vertices");
  }

  for(auto it = indices.begin(); it != indices.end(); it++) {
    Vertex vertex;
    vertex.position = vertices[it->at(0)];
    if(!uvs.empty()) {
      vertex.uv = uvs[it->at(1)];
    }
    if(!normals.empty()) {
      vertex.normal = normals[it->at(2)];
    }
    this->vertices.push_back(vertex);
  }
}

std::vector<std::array<uint32_t, 3>> Mesh::collectFaces(std::istringstream& lineStream) {
  uint32_t index = 0;
  std::vector<std::array<uint32_t, 3>> result;
  std::array<uint32_t, 3> face {0};
  while(lineStream >> face[0]) {
    face[0]--;
    if(lineStream.peek() == '/') {
      lineStream.get();
      lineStream >> face[1];
      face[1]--;
      if(lineStream.peek() == '/') {
        lineStream.get();
        lineStream >> face[2];
        face[2]--;
      }
    } 
    result.push_back(face);
  }
  return result;
}

void Mesh::loadMaterial(const std::string& path) {
  std::ifstream materialFile(path);
  std::optional<std::string> currentMaterialName;

  std::string line;
  while(std::getline(materialFile, line)) {
    std::istringstream lineStream(line);
    std::string lineStart;
    lineStream >> lineStart;

    if(lineStart == "newmtl") {
      std::puts("before lineStream >> l");
      std::string materialName;
      lineStream >> materialName;
      currentMaterialName = std::move(materialName);
      usedTexture = currentMaterialName.value();
      std::puts("after lineStream >> l");
    } else if(lineStart == "map_Kd") {
      std::string textureName;
      lineStream >> textureName;

      textures[currentMaterialName.value()].loadFromFile("assets/textures/" + textureName);
    } else {
      // skip line
    }
  }
  materialFile.close();
}