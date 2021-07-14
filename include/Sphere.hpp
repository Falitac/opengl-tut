#pragma once

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

namespace generators {

void generateSphere(
  std::vector<glm::vec3>& vertices,
  std::vector<uint32_t>& indices,
  std::vector<glm::vec3>& normals,
  std::vector<glm::vec2>& uvs,
  float radius,
  uint32_t verticalSubdivisions,
  uint32_t horizontalSubdivisions
);

} // namespace project