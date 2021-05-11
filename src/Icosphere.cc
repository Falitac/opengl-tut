#include "../include/Icosphere.hpp"

#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>
#include <array>

namespace generators {

void generateIcosphere(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  float radius,
  int divisions
) {
  std::vector<float>().swap(vertices);
  vertices.reserve(12);
  auto dphi = glm::two_pi<float>() / 5.0f;
  auto latiduteHeight = radius * std::sin(std::atan2(1.0f, 2.0f));
  auto middleRadius = std::sqrt(radius*radius - latiduteHeight*latiduteHeight);

  vertices.emplace_back(0.0f);
  vertices.emplace_back(radius);
  vertices.emplace_back(0.0f);


  for(int i = 0; i < 5; i++) {
    auto phi = i * dphi;
    auto phiPlus18 = phi + glm::two_pi<float>() / 10.0f;
  
    vertices.emplace_back(middleRadius * std::cos(phi));
    vertices.emplace_back(latiduteHeight);
    vertices.emplace_back(middleRadius * std::sin(phi));
    
    vertices.emplace_back(middleRadius * std::cos(phiPlus18));
    vertices.emplace_back(-latiduteHeight);
    vertices.emplace_back(middleRadius * std::sin(phiPlus18));
  }


  vertices.emplace_back(0.0f);
  vertices.emplace_back(-radius);
  vertices.emplace_back(0.0f);

  // indices

  for(int i = 0; i < 5; i++) {
    // top
    indices.emplace_back(0);
    indices.emplace_back((i + 1) % 5 * 2 + 1);
    indices.emplace_back(i * 2 + 1);

    // upper mid
    indices.emplace_back(i * 2 + 1);
    indices.emplace_back((i + 1) % 5 * 2 + 1);
    indices.emplace_back(i * 2 + 2);

    // lower mid
    indices.emplace_back(i * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 1);

    // bottom
    indices.emplace_back(11);
    indices.emplace_back(i * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 2);
  }
  auto corectVertex = [&radius] (std::array<float, 3>& point) {
    std::array<float, 3> p;
    auto R = point[0]*point[0] + point[1]*point[1] + point[2]*point[2];
    R = std::sqrt(R);
    p[0] = point[0] / R * radius;
    p[1] = point[1] / R * radius;
    p[2] = point[2] / R * radius;
    point = p;
  };

  for(int i = 1; i <= divisions; i++) {
    std::vector<float> newVertices;
    std::vector<uint32_t> newIndices;
    newVertices.reserve(vertices.size() * 2);
    newIndices.reserve(indices.size() * 2);
    
    uint32_t indicesIt = 0;

    for(int j = 0; j < indices.size(); j+=3, indicesIt += 6) { // for each triangle
      std::array<float, 3> newVertex;
      for(int k = 0; k < 3; k++) { //for each vertex of triangle
        uint32_t index1 = indices[j + k]*3;
        uint32_t index2 = indices[(j + k + 1) % 3]*3;
        newVertices.emplace_back(vertices[index1 + 0]);
        newVertices.emplace_back(vertices[index1 + 1]);
        newVertices.emplace_back(vertices[index1 + 2]);

        newVertex[0] = (vertices[index1 + 0] + vertices[index2 + 0]) / 2.0f;
        newVertex[1] = (vertices[index1 + 1] + vertices[index2 + 1]) / 2.0f;
        newVertex[2] = (vertices[index1 + 2] + vertices[index2 + 2]) / 2.0f;
        corectVertex(newVertex);
        newVertices.insert(newVertices.end(), newVertex.begin(), newVertex.end());

      }
      newIndices.emplace_back(indicesIt + 0);
      newIndices.emplace_back(indicesIt + 1);
      newIndices.emplace_back(indicesIt + 5);

      newIndices.emplace_back(indicesIt + 2);
      newIndices.emplace_back(indicesIt + 3);
      newIndices.emplace_back(indicesIt + 1);

      newIndices.emplace_back(indicesIt + 4);
      newIndices.emplace_back(indicesIt + 5);
      newIndices.emplace_back(indicesIt + 3);

      newIndices.emplace_back(indicesIt + 5);
      newIndices.emplace_back(indicesIt + 1);
      newIndices.emplace_back(indicesIt + 3);
    }
    vertices = std::move(newVertices);
    indices = std::move(newIndices);
  }
}

} //namespace generators