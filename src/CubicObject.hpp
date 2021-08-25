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

template<unsigned n_faces>
class CubicObject : public GameObject<CubeVertex>{
private:
    using SuperClass = GameObject<CubeVertex>;
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

    CubicObject(const Item& item, const TileBlock& visible_faces, const glm::mat4& transform);

    static const PositionsMatrix local_vertex_positions;
    static const UvsMatrix uvs;
    static const IndicesMatrix indices;
    //static const IndicesMatrix flipped;
    static const NormalMatrix normals;
};

using Plant = CubicObject<4>;
using Cube = CubicObject<6>;

#endif //CPP_CUBICOBJECT_HPP
