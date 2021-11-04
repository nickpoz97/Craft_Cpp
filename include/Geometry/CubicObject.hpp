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
#include "tunableParameters.hpp"

namespace CraftCpp {
static constexpr int INDICES_FACE_COUNT = 6;

using CubeVertexIterator = typename std::vector<CubeVertex>::iterator;

template<unsigned nFaces>
class CubicObject {
private:
    CubeVertexIterator beginIterator;
    size_t nVertices;
    static constexpr float N = 0.5f;

    static glm::vec3 rotate_asy(const glm::vec3 &v, float angle_degrees);
public:
    using PositionsMatrix = std::array<std::array<glm::vec3, 4>, nFaces>;
    using IndicesMatrix = std::array<std::array<int, INDICES_FACE_COUNT>, nFaces>;
    using NormalMatrix = std::array<glm::vec3, nFaces>;
    using UvsMatrix = std::array<std::array<glm::bvec2, 4>, nFaces>;

    static constexpr float S = 0.0625;
    static const float A;
    static const float B;

    static constexpr int maxIndices = INDICES_FACE_COUNT * nFaces;

    CubicObject(const BlockType &blockType, const std::array<bool, 6> &visibleFaces,
                const glm::vec3 &centerPosition,
                float asyRotation, const std::unordered_map<glm::ivec3, bool> &lightObstacles,
                CubeVertexIterator verticesIt);

    static const PositionsMatrix localVertexPositions;
    static const UvsMatrix uvs;
    static const IndicesMatrix indices;
    static const NormalMatrix normals;

    [[nodiscard]] CubeVertexIterator end() const;

    [[nodiscard]] CubeVertexIterator begin() const;

    void print_vertex_info();
};

class Plant : public CubicObject<4> {
private:
    using super = CubicObject<4>;
public:
    Plant(const BlockType &blockType, const glm::vec3 &centerPosition, float asy_rotation,
          CubeVertexIterator verticesIt, const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};

class Cube : public CubicObject<6> {
private:
    using super = CubicObject<6>;
public:
    Cube(const BlockType &blockType, const std::array<bool, 6> &visibleFaces, const glm::vec3 &position,
         CubeVertexIterator verticesIt, const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};
}
#endif //CPP_CUBICOBJECT_HPP
