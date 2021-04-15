#include "../include/Cube.hpp"

#include <cmath>
#include <iostream>

namespace generators {

void generateCube(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  float side
) {
  vertices.reserve(8 * 3);

  auto halfSide = side / 2.0f;

  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      for(int k = 0; k < 2; k++) {
        vertices.emplace_back(halfSide * (i % 2 ? 1 : -1));
        vertices.emplace_back(halfSide * (j % 2 ? 1 : -1));
        vertices.emplace_back(halfSide * (k % 2 ? 1 : -1));
      }
    }
  }
  indices.emplace_back(0);
  indices.emplace_back(1);
  indices.emplace_back(2);
}

} //namespace generators