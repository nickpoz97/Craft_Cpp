//
// Created by ultimatenick on 07/08/21.
//

#include <numeric>
#include <forward_list>

#include "fmt/ostream.h"
#include "glm/trigonometric.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/vec3.hpp"
#include "Geometry/BlockObject.hpp"
#include "Geometry/map_utils.hpp"

namespace CraftCpp {
template<>
const float BlockObject<6>::A = 0.0 + 1 / 2048.0;

template<>
const float BlockObject<6>::B = S - 1 / 2048.0;

template<>
const float BlockObject<4>::A = 0.0;

template<>
const float BlockObject<4>::B = S;

template<unsigned n_faces>
BlockObject<n_faces>::BlockObject(const BlockType &blockType, const std::array<bool, 6> &visibleFaces,
                                  const glm::vec3 &centerPosition,
                                  float asyRotation,
                                  const std::unordered_map<glm::ivec3, bool> &lightObstacles,
                                  BlockVertexIterator verticesIt) :
        beginIterator{verticesIt},
        nVertices{static_cast<size_t>(std::accumulate(visibleFaces.begin(), visibleFaces.end(), 0) *
                                      INDICES_FACE_COUNT)} {
    TileBlock tile_block{blockType};

    auto face_v_it = localVertexPositions.begin();
    auto face_uvs_it = uvs.begin();
    auto face_nrm_it = normals.begin();
    auto tile_it = tile_block.begin();
    auto increment_geometry_iterators = [&]() {
        ++face_v_it;
        ++face_uvs_it;
        ++face_nrm_it;
        ++tile_it;
    };

    auto visible_faces_it = visibleFaces.begin();

    for (auto face_ind_it{indices.begin()}; face_ind_it != indices.end(); ++face_ind_it) {
        // test if face is visible
        if (!*(visible_faces_it++)) {
            if (n_faces != 4) {
                increment_geometry_iterators();
            } else {
                ++tile_it;
            }
            continue;
        }
        float du = static_cast<float>(*tile_it % 16) * S;
        float dv = static_cast<float>(*tile_it / 16) * S;

        //obtain arrays of actual face elements
        const auto &face_vertices = *(face_v_it);
        const glm::vec3 &face_normal = *(face_nrm_it);
        const auto &face_uvs = *(face_uvs_it);
        // iterate through actual face indices
        for (int i: *face_ind_it) {
            auto &actual_vertex = *(verticesIt++);
            // initialize texture coordinates
            // obtain local(cube or flower) coordinates
            actual_vertex.position = centerPosition +
                                     glm::rotateY(N * face_vertices[i], glm::radians(asyRotation));
            // assign normal
            actual_vertex.normal = face_normal;
            actual_vertex.uv = {
                    du + (face_uvs[i].x ? B : A),
                    dv + (face_uvs[i].y ? B : A)
            };
#ifndef NDEBUG
            assert(lightObstacles.size() == 0 || lightObstacles.size() == 27);
#endif

            actual_vertex.ao = 1.0f;

            if (!lightObstacles.empty()) {
                std::forward_list<glm::ivec3> obstaclesCoords{};

                glm::ivec3 directions = face_vertices[i] - face_normal;
                if (directions.x != 0) obstaclesCoords.push_front(face_normal + glm::vec3{directions.x, 0, 0});
                if (directions.y != 0) obstaclesCoords.push_front(face_normal + glm::vec3{0, directions.y, 0});
                if (directions.z != 0) obstaclesCoords.push_front(face_normal + glm::vec3{0, 0, directions.z});
                if (directions.x != 0 && directions.z != 0) {
                    obstaclesCoords.push_front(face_normal + glm::vec3{directions.x, 0, directions.z});
                }
                for (const auto &aoCoord: obstaclesCoords) {
                    actual_vertex.ao += actual_vertex.ao * lightObstacles.at(static_cast<glm::ivec3>(aoCoord));
                }
            }
            actual_vertex.ao = 1 / actual_vertex.ao;
        }
        increment_geometry_iterators();
    }
}

template<unsigned int n_faces>
BlockVertexIterator BlockObject<n_faces>::end() const {
    return beginIterator + nVertices;
}

template<unsigned int n_faces>
glm::vec3 BlockObject<n_faces>::rotate_asy(const glm::vec3 &v, float angle_degrees) {
    return {
            glm::cos(angle_degrees) * v.x - glm::sin(angle_degrees) * v.z,
            v.y,
            glm::sin(angle_degrees) * v.x + glm::cos(angle_degrees) * v.z,
    };
}

Cube::Cube(const BlockType &blockType, const std::array<bool, 6> &visibleFaces, const glm::vec3 &position,
           BlockVertexIterator verticesIt, const std::unordered_map<glm::ivec3, bool> &lightObstacles) :
        super(blockType, visibleFaces, position, 0, lightObstacles, verticesIt) {}

template<unsigned n_faces>
void BlockObject<n_faces>::printVertexInfo() {
    int index = 0;
    for (auto it = begin(); it != end(); ++it) {
        fmt::print("pos: ({},{},{}), face: {}, normal: ({},{},{})\n", it->position.x, it->position.y,
                   it->position.z,
                   (index++) / INDICES_FACE_COUNT, it->normal.x, it->normal.y, it->normal.z);
    }
}

template<>
const BlockObject<6>::PositionsMatrix BlockObject<6>::localVertexPositions{{
         {{{-1, -1, -1}, {-1, -1, +1},
           {-1, +1, -1}, {-1, +1, +1}}},
         {{{+1, -1, -1}, {+1, -1, +1},
           {+1, +1, -1}, {+1, +1, +1}}},
         {{{-1, +1, -1}, {-1, +1, +1},
           {+1, +1, -1}, {+1, +1, +1}}},
         {{{-1, -1, -1}, {-1, -1, +1},
           {+1, -1, -1}, {+1, -1, +1}}},
         {{{-1, -1, -1}, {-1, +1, -1},
           {+1, -1, -1}, {+1, +1, -1}}},
         {{{-1, -1, +1}, {-1, +1, +1},
           {+1, -1, +1}, {+1, +1, +1}}}
 }};

template<>
const BlockObject<6>::NormalMatrix BlockObject<6>::normals{{
       {-1, 0, 0},
       {+1, 0, 0},
       {0, +1, 0},
       {0, -1, 0},
       {0, 0, -1},
       {0, 0, +1}
}};

template<>
const BlockObject<6>::IndicesMatrix BlockObject<6>::indices{{
        {0, 3, 2, 0, 1, 3},
        {0, 3, 1, 0, 2, 3},
        {0, 3, 2, 0, 1, 3},
        {0, 3, 1, 0, 2, 3},
        {0, 3, 2, 0, 1, 3},
        {0, 3, 1, 0, 2, 3}
}};

template<>
const BlockObject<6>::UvsMatrix BlockObject<6>::uvs{{
        {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
        {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}},
        {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}},
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
        {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}}
}};

template<unsigned int n_faces>
BlockVertexIterator BlockObject<n_faces>::begin() const {
    return beginIterator;
}

template<>
const BlockObject<4>::PositionsMatrix BlockObject<4>::localVertexPositions{{
         {{{0, -1, -1}, {0, -1, +1},
           {0, +1, -1}, {0, +1, +1}}},
         {{{0, -1, -1}, {0, -1, +1},
           {0, +1, -1}, {0, +1, +1}}},
         {{{-1, -1, 0}, {-1, +1, 0},
           {+1, -1, 0}, {+1, +1, 0}}},
         {{{-1, -1, 0}, {-1, +1, 0},
           {+1, -1, 0}, {+1, +1, 0}}}
 }};

template<>
const BlockObject<4>::NormalMatrix BlockObject<4>::normals{{
       {-1, 0, 0},
       {+1, 0, 0},
       {0, 0, -1},
       {0, 0, +1}
}};

template<>
const BlockObject<4>::IndicesMatrix BlockObject<4>::indices{{
        {0, 3, 2, 0, 1, 3},
        {0, 3, 1, 0, 2, 3},
        {0, 3, 2, 0, 1, 3},
        {0, 3, 1, 0, 2, 3}
}};

template<>
const BlockObject<4>::UvsMatrix BlockObject<4>::uvs{{
        {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
        {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}},
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
        {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}}
}};

template
class BlockObject<6>;

template
class BlockObject<4>;

Plant::Plant(const BlockType &blockType, const glm::vec3 &centerPosition, float asy_rotation,
             BlockVertexIterator verticesIt, const std::unordered_map<glm::ivec3, bool> &lightObstacles) :
        super{blockType, {1, 1, 0, 0, 1, 1}, centerPosition, asy_rotation, lightObstacles, verticesIt} {}
}