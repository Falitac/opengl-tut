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
    indices.emplace_back(i * 2 + 1);
    indices.emplace_back((i + 1) % 5 * 2 + 1);

    // upper mid
    indices.emplace_back(i * 2 + 1);
    indices.emplace_back(i * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 1);

    // lower mid
    indices.emplace_back(i * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 1);

    // bottom
    indices.emplace_back(11);
    indices.emplace_back(i * 2 + 2);
    indices.emplace_back((i + 1) % 5 * 2 + 2);
  }

  for(int i = 0; i < indices.size(); i+=3) {
    printf("T %d:\n", i / 3);
    for(int j = 0; j < 3; j++) {
      printf("%f %f %f\n",
        vertices[indices[i+j]*3 + 0],
        vertices[indices[i+j]*3 + 1],
        vertices[indices[i+j]*3 + 2]
      );
    }
  }

  for(int i = 1; i <= divisions; i++) {
    std::vector<float> newVertices;
    std::vector<uint32_t> newIndices;
    newVertices.reserve(vertices.size() * 2);
    newIndices.reserve(indices.size() * 2);

    for(int j = 0; j < indices.size(); j += 3) { // for each triangle
      std::array<float, 3> newVertex;
      for(int k = 0; k < 3; k++) { //for each vertex of triangle
        newVertices.emplace_back(vertices[indices[j + k]*3 + 0]);
        newVertices.emplace_back(vertices[indices[j + k]*3 + 1]);
        newVertices.emplace_back(vertices[indices[j + k]*3 + 2]);

        newVertex[0] = (vertices[indices[j + k]*3 + 0] + vertices[indices[(j + k + 1) % 3]*3 + 0]) / 2.0f;
        newVertex[1] = (vertices[indices[j + k]*3 + 1] + vertices[indices[(j + k + 1) % 3]*3 + 1]) / 2.0f;
        newVertex[2] = (vertices[indices[j + k]*3 + 2] + vertices[indices[(j + k + 1) % 3]*3 + 2]) / 2.0f;
        newVertices.insert(newVertices.end(), newVertex.begin(), newVertex.end());

        std::printf("%.5f %.5f %.5f\n", newVertex[0], newVertex[1], newVertex[0]);
      }
      newIndices.emplace_back(j + 0);
      newIndices.emplace_back(j + 1);
      newIndices.emplace_back(j + 2);

    }
    vertices = std::move(newVertices);
    indices = std::move(newIndices);
  }
}

} //namespace generators