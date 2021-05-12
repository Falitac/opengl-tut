#include "../include/Sphere.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>

namespace generators {


void generateSphere(
  std::vector<float>& vertices,
  std::vector<uint32_t>& indices,
  std::vector<float>& normals,
  float radius,
  int divisions
) {
  auto calculateNormal = [&indices, &vertices] (uint32_t a, uint32_t b, uint32_t c) -> glm::vec3 {
    glm::vec3 vec_a {
      vertices[a*3 + 0],
      vertices[a*3 + 1],
      vertices[a*3 + 2]
    };
    glm::vec3 vec_b {
      vertices[b*3 + 0],
      vertices[b*3 + 1],
      vertices[b*3 + 2]
    };
    glm::vec3 vec_c {
      vertices[c*3 + 0],
      vertices[c*3 + 1],
      vertices[c*3 + 2]
    };
    auto ba = vec_b - vec_a;
    auto cb = vec_c - vec_b;
    return glm::normalize(glm::cross(ba, cb));
  };
  auto addNormalForLast3 = [&normals, &indices, &calculateNormal] {
    glm::vec3 normal = calculateNormal(
      indices[indices.size() - 3],
      indices[indices.size() - 2],
      indices[indices.size() - 1]
    );
    normals.emplace_back(normal.x);
    normals.emplace_back(normal.y);
    normals.emplace_back(normal.z);
  };

  vertices.emplace_back(0.0f);
  vertices.emplace_back(-radius);
  vertices.emplace_back(0.0f);

  for(int i = 1; i < divisions; i++) {
    auto theta = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(divisions) - glm::half_pi<float>();
    for(int j = 0; j < divisions; j++) {
      auto phi = glm::two_pi<float>() * static_cast<float>(j) / static_cast<float>(divisions);
      vertices.emplace_back(radius * std::cos(phi) * std::cos(theta));
      vertices.emplace_back(radius * std::sin(theta));
      vertices.emplace_back(radius * std::sin(phi) * std::cos(theta));
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
    addNormalForLast3();
  }

  // rest
  for(int i = 0; i < divisions - 2; i++) {
    for(int j = 0; j < divisions; j++) {
      indices.emplace_back(j + 1 + i * divisions);
      indices.emplace_back((j + 1) % divisions + 1 + (i + 1) * divisions);
      indices.emplace_back((j + 1) % divisions + 1 + i * divisions);
      addNormalForLast3();

      indices.emplace_back(j + 1 + i * divisions);
      indices.emplace_back(j + 1 + (i + 1) * divisions);
      indices.emplace_back((j + 1) % divisions + 1 + (i + 1) * divisions);
      addNormalForLast3();
    }
  }

  // upper vertex
  auto N = vertices.size() / 3 - 1;
  for(int i = 0; i < divisions; i++) {
    indices.emplace_back(N);
    indices.emplace_back(N - 1 - i);
    indices.emplace_back(N - 1 - (i + 1) % divisions);
    addNormalForLast3();
  }
}

} // namespace generators