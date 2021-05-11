#pragma once

#include <vector>
#include <cstdint>

namespace generators {

void generateSphere(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  std::vector<float>& normals,
  float radius = 6.0f,
  int divisions = 50
);

} // namespace project