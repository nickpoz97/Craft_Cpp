//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_CUBICOBJECT_HPP
#define CPP_CUBICOBJECT_HPP

#include <vector>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <array>

#include "StandardVertex.hpp"
#include "GameObject.hpp"

static constexpr int VERTICES_FACE_COUNT = 6;

template<int n_faces, int n_vertices_face>
class CubicObject : GameObject<CubeVertex, n_faces * n_vertices_face>{
private:
    using SuperClass = GameObject<CubeVertex, n_faces * n_vertices_face>;

    static constexpr float N = 0.5f;
    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic

    bool is_cube = (n_faces == 6);

    static const std::array<glm::vec3, n_faces> normals;
    static const std::array<std::array<int, n_vertices_face>, n_faces> indices;
    static const std::array<std::array<glm::vec3, 4>, n_faces> local_vertex_positions;
    static const std::array<std::array<glm::bvec2, 4>, n_faces> uvs;
    static const std::array<std::array<float, 4>, n_faces> ao;
    static const std::array<std::array<float, 4>, n_faces> light;

public:
    CubicObject(const TileBlock& tiles);
    std::array<int, n_vertices_face> get_face_indices(int face_index, std::array<float, 4> ao_face);
    void apply_transform(const glm::mat4 &transform);
};

#endif //CPP_CUBICOBJECT_HPP
