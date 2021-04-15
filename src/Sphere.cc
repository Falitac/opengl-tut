#include "../include/Sphere.hpp"

#include <glm/gtc/constants.hpp>
#include <cmath>

namespace generators {

void generateSphere(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  float radius,
  int divisions
) {
  float dr = radius / static_cast<float>(divisions);
  const auto dphi = glm::two_pi<float>() / static_cast<float>(divisions);
  

  vertices.emplace_back(0.0f);
  vertices.emplace_back(-radius);
  vertices.emplace_back(0.0f);

  for(float height = -radius  + dr; height <= radius  - dr; height += dr) {
    for(int i = 0; i < divisions; i++) {
      auto phi = i * dphi;
      auto actualRadius = std::sqrt(radius*radius - height*height);
      vertices.emplace_back(actualRadius * std::cos(phi));
      vertices.emplace_back(height);
      vertices.emplace_back(actualRadius * std::sin(phi));
    }
  }

  vertices.emplace_back(0.0f);
  vertices.emplace_back(radius);
  vertices.emplace_back(0.0f);

  // indices 

  // bottom vertex
  for(int i = 0; i < divisions; i++) {
    indices.emplace_back(0);
    indices.emplace_back((i + 1) % (divisions + 1));
    indices.emplace_back((i + 1) % divisions + 1);
  }

  // rest
  for(int j = 0; j < 2 * (divisions - 1); j++) {
    for(int i = 0; i < divisions; i++) {
      indices.emplace_back(i + 1 + j * divisions);
      indices.emplace_back((i + 1) % divisions + 1 + j * divisions);
      indices.emplace_back((i + 1) % divisions + 1 + (j + 1) * divisions);

      indices.emplace_back(i + 1 + j * divisions);
      indices.emplace_back((i + 1) % divisions + 1 + (j + 1) * divisions);
      indices.emplace_back(i + 1 + (j + 1) * divisions);
    }
  }

  // upper vertex
  for(int i = 0; i < divisions; i++) {
    auto N = vertices.size() / 3 - 1;
    indices.emplace_back(N);
    indices.emplace_back(N - 1 - (i + 1) % divisions);
    indices.emplace_back(N - 1 - i);
  }

}

} // namespace generators