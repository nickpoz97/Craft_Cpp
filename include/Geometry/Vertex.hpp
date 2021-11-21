//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_VERTEX_HPP
#define CPP_VERTEX_HPP

#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace CraftCpp {
struct Standard2DVertex {
  glm::vec2 position;
};

struct Uv2DVertex : Standard2DVertex {
  glm::vec2 uv;
};

struct Standard3DVertex {
  glm::vec3 position;
};

struct Uv3DVertex : Standard3DVertex {
  glm::vec2 uv;
};

struct NormalVertex : Uv3DVertex {
  glm::vec3 normal;
};

class BlockVertex : public NormalVertex {
public:
  float ao;
};
} // namespace CraftCpp

#endif // CPP_VERTEX_HPP
