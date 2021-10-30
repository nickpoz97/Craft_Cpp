//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_CUBICOBJECT_HPP
#define CPP_CUBICOBJECT_HPP

#include <vector>
#include <array>
#include <unordered_map>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Geometry/Vertex.hpp"
#include "Geometry/RenderableEntity.hpp"
#include "tunable_parameters.hpp"

namespace CraftCpp {
using cube_vertex_iterator_t = typename std::vector<CubeVertex>::iterator;

template<unsigned n_faces>
class CubicObject {
private:
    cube_vertex_iterator_t begin_iterator;
    size_t n_vertices;

    static glm::vec3 rotate_asy(const glm::vec3 &v, float angle_degrees);

public:
    using PositionsMatrix = std::array<std::array<glm::vec3, 4>, n_faces>;
    using IndicesMatrix = std::array<std::array<int, INDICES_FACE_COUNT>, n_faces>;
    using NormalMatrix = std::array<glm::vec3, n_faces>;
    using UvsMatrix = std::array<std::array<glm::bvec2, 4>, n_faces>;

    static constexpr float S = 0.0625;
    static const float A;
    static const float B;

    static constexpr int max_indices = INDICES_FACE_COUNT * n_faces;

    CubicObject(const BlockType &block_type, const std::array<bool, 6> &visible_faces,
                const glm::vec3 &center_position,
                float asy_rotation, const std::unordered_map<glm::ivec3, bool> &lightObstacles,
                cube_vertex_iterator_t vertices_it);

    static const PositionsMatrix local_vertex_positions;
    static const UvsMatrix uvs;
    static const IndicesMatrix indices;
    static const NormalMatrix normals;

    [[nodiscard]] cube_vertex_iterator_t end() const;

    [[nodiscard]] cube_vertex_iterator_t begin() const;

    void print_vertex_info();
};

class Plant : public CubicObject<4> {
private:
    using super = CubicObject<4>;
public:
    Plant(const BlockType &block_type, const glm::vec3 &center_position, float asy_rotation,
          cube_vertex_iterator_t vertices_it, const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};

class Cube : public CubicObject<6> {
private:
    using super = CubicObject<6>;
public:
    Cube(const BlockType &block_type, const std::array<bool, 6> &visible_faces, const glm::vec3 &position,
         cube_vertex_iterator_t vertices_it, const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};
}
#endif //CPP_CUBICOBJECT_HPP
