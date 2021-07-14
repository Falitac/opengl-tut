#include "../include/Sphere.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>

namespace generators {


void generateSphere(
  std::vector<glm::vec3>& vertices,
  std::vector<uint32_t>& indices,
  std::vector<glm::vec3>& normals,
  std::vector<glm::vec2>& uvs,
  float radius,
  uint32_t verticalSubdivisions,
  uint32_t horizontalSubdivisions
) {
  vertices.emplace_back(0.0f, -radius, 0.0f);
  normals.emplace_back(0.0f, -1.0f, 0.0f);
  for(int i = 1; i < verticalSubdivisions; i++) {
    auto theta = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(verticalSubdivisions) - glm::half_pi<float>();
    for(int j = 0; j < horizontalSubdivisions; j++) {
      auto phi = glm::two_pi<float>() * static_cast<float>(j) / static_cast<float>(horizontalSubdivisions);
      auto v = glm::vec3(
        std::cos(phi) * std::cos(theta),
        std::sin(theta),
        std::sin(phi) * std::cos(theta)
      );
      normals.push_back(v);
      v *= radius;
      vertices.push_back(std::move(v));

      uvs.emplace_back(
        1.0f - static_cast<float>(j)/horizontalSubdivisions,
        1.0f - static_cast<float>(i)/verticalSubdivisions
      );
    }
  }
  vertices.emplace_back(0.0f, radius, 0.0f);
  normals.emplace_back(0.0f, 1.0f, 0.0f);

  // bottom vertex
  for(int i = 0; i < horizontalSubdivisions; i++) {
    indices.emplace_back(0);
    indices.emplace_back((i + 1) % (horizontalSubdivisions + 1));
    indices.emplace_back((i + 1) % horizontalSubdivisions + 1);
  }

  // rest
  for(int i = 0; i < verticalSubdivisions - 2; i++) {
    for(int j = 0; j < horizontalSubdivisions; j++) {
      indices.emplace_back(j + 1 + i * horizontalSubdivisions);
      indices.emplace_back((j + 1) % horizontalSubdivisions + 1 + (i + 1) * horizontalSubdivisions);
      indices.emplace_back((j + 1) % horizontalSubdivisions + 1 + i * horizontalSubdivisions);

      indices.emplace_back(j + 1 + i * horizontalSubdivisions);
      indices.emplace_back(j + 1 + (i + 1) * horizontalSubdivisions);
      indices.emplace_back((j + 1) % horizontalSubdivisions + 1 + (i + 1) * horizontalSubdivisions);
    }
  }

  // upper vertex
  auto N = vertices.size() - 1;
  for(int i = 0; i < horizontalSubdivisions; i++) {
    indices.emplace_back(N);
    indices.emplace_back(N - 1 - i);
    indices.emplace_back(N - 1 - (i + 1) % horizontalSubdivisions);
  }
}

} // namespace generators