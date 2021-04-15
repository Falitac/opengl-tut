#pragma once

#include <vector>
#include <cstdint>

namespace generators {

void generateCube(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  float side = 5.f
);

} // namespace generators