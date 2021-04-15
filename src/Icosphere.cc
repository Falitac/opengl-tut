#include "../include/Icosphere.hpp"

#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>

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
  auto latiduteHeight = radius * std::atan2(1.0f, 2.0f);
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
}

} //namespace generators