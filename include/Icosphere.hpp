#pragma once

#include <vector>
#include <cstdint>

namespace generators {

void generateIcosphere(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  float radius = 5.f,
  int divisions = 1
);

} // namespace generators