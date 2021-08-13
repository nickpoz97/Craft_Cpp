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
    const bool is_cube = (n_faces == 6);

protected:
    using LightMatrix = std::array<std::array<float, 4>, n_faces>;
    using PositionsMatrix = std::array<std::array<glm::vec3, 4>, n_faces>;
    using IndicesMatrix = std::array<std::array<int, n_vertices_face>, n_faces>;
    using NormalMatrix = std::array<glm::vec3, n_faces>;
    using UvsMatrix = std::array<std::array<glm::bvec2, 4>, n_faces>;

public:
    CubicObject(const TileBlock& tiles,
                const LightMatrix& ao,
                const LightMatrix& light,
                const PositionsMatrix& local_vertex_positions,
                const IndicesMatrix& indices,
                const NormalMatrix& normals,
                const UvsMatrix& uvs,
                float s,
                float a,
                float b
                );
};

#endif //CPP_CUBICOBJECT_HPP
