//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_CUBICOBJECT_HPP
#define CPP_CUBICOBJECT_HPP

#include <vector>
#include <array>

#include "vec3.hpp"
#include "mat4x4.hpp"
#include "Vertex.hpp"
#include "GameObject.hpp"
#include "Chunk.hpp"

using cube_vertex_iterator_t = typename std::vector<CubeVertex>::iterator;

template<unsigned n_faces>
class CubicObject{
private:
    glm::mat4 transform_matrix{1.0};
    cube_vertex_iterator_t begin;
    size_t n_vertices;

protected:
    //using LightMatrix = std::array<std::array<float, 4>, n_faces>;
    using PositionsMatrix = std::array<std::array<glm::vec3, 4>, n_faces>;
    using IndicesMatrix = std::array<std::array<int, INDICES_FACE_COUNT>, n_faces>;
    using NormalMatrix = std::array<glm::vec3, n_faces>;
    using UvsMatrix = std::array<std::array<glm::bvec2, 4>, n_faces>;
public:
    //using IlluminationVector = std::array<float, 27>;

    static constexpr float S = 0.0625;              // TODO check semantic
    static constexpr float A = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float B = S - 1 / 2048.0;      // TODO check semantic

    CubicObject(const BlockType& block_type, const std::array<bool, 6> &visible_faces, const glm::mat4& transform, cube_vertex_iterator_t vertices_it);

    static const PositionsMatrix local_vertex_positions;
    static const UvsMatrix uvs;
    static const IndicesMatrix indices;
    //static const IndicesMatrix flipped;
    static const NormalMatrix normals;

    cube_vertex_iterator_t get_end() const;
};

class Plant : public CubicObject<4>{
private:
    using super = CubicObject<4>;
public:
    Plant(const BlockType& block_type, const std::array<bool, 6> &visible_faces, const glm::vec3& position,
          float rotation, cube_vertex_iterator_t vertices_it);

    static glm::mat4 get_transform_matrix(const glm::vec3& position, float rotation);
};

class Cube : public CubicObject<6>{
private:
    using super = CubicObject<6>;
public:
    Cube(const BlockType& block_type, const std::array<bool, 6> &visible_faces, const glm::vec3& position,
         cube_vertex_iterator_t vertices_it);
    static glm::mat4 get_transform_matrix(const glm::vec3& position);
};

#endif //CPP_CUBICOBJECT_HPP
