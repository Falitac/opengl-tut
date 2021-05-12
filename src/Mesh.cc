#include "../include/Mesh.hpp"

#include <filesystem>
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
      glEnableVertexAttribArray(1);

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

      glBindVertexArray(0);
    }
  }
  shader.loadFromFile("assets/shaders/basic-v.glsl", "assets/shaders/basic-v.glsl");
}

void Mesh::update() {

}

void Mesh::draw() {
  sf::Shader::bind(&shader);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
}

void Mesh::genBuffers() {
  glGenBuffers(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &uvo);
  glGenBuffers(1, &nvo);
}

void Mesh::deleteBuffers() {
  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &uvo);
  glDeleteBuffers(1, &nvo);
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
      if(faces.size() > 3) {
        auto first = *faces.begin();
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
      std::printf("found material %s\n", materialPath.c_str());
    } else if (lineStart == "usemtl") {
      std::string materialName;
      lineStream >> materialName;
      std::printf("used material %s\n", materialName.c_str());
    } else if (lineStart == "s") {
      std::string smoothShading;
      lineStream >> smoothShading;
      std::printf("smooth shading: %s\n", smoothShading.c_str());
    } else if (lineStart == "#") {
    } else {
      std::puts("unknown thing");
    }
  }
  std::printf("reading done!\n");

  for(auto& index : indices) {
    std::printf("%llu %llu %llu\n", index[0], index[1], index[2]);
  }
  for(auto it = indices.begin(); it != indices.end(); it++) {
    Vertex vertex;
    vertex.position = vertices[it->at(0)];
    vertex.uv = uvs[it->at(1)];
    vertex.position = normals[it->at(2)];
    this->vertices.push_back(vertex);
  }
}

std::vector<std::array<uint32_t, 3>> Mesh::collectFaces(std::istringstream& lineStream) {
  uint32_t index;
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