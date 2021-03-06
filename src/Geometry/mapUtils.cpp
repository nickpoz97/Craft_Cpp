//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "Geometry/Chunk.hpp"
#include "Geometry/mapUtils.hpp"

namespace CraftCpp {
std::size_t hashCoefficient(int val) {
  auto key = static_cast<std::size_t>(val);
  key = ~key + (key << 15);
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057;
  key = key ^ (key >> 16);
  return key;
}

bool yCoordComparator::operator()(const glm::vec3 &a,
                                  const glm::vec3 &b) const {
  return a.y < b.y;
}
} // namespace CraftCpp

namespace std {
size_t hash<glm::ivec3>::operator()(const glm::ivec3 &v) const {
  return CraftCpp::hashCoefficient(v.x) ^ CraftCpp::hashCoefficient(v.y) ^
         CraftCpp::hashCoefficient(v.z);
}

size_t hash<glm::ivec2>::operator()(const glm::ivec2 &v) const {
  return CraftCpp::hashCoefficient(v.x) ^ CraftCpp::hashCoefficient(v.y);
}
} // namespace std
