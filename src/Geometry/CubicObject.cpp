//
// Created by ultimatenick on 07/08/21.
//

#include <numeric>
#include <fmt/ostream.h>
#include <gtx/rotate_vector.hpp>

#include "vec3.hpp"
#include "CubicObject.hpp"
#include "../costants.hpp"
#include "trigonometric.hpp"

template<unsigned n_faces>
CubicObject<n_faces>::CubicObject(const BlockType &block_type, const std::array<bool, 6> &visible_faces,
                                  const glm::vec3 &center_position,
                                  float asy_rotation, cube_vertex_iterator_t vertices_it) :
        begin{vertices_it},
        n_vertices{static_cast<size_t>(std::accumulate(visible_faces.begin(), visible_faces.end(), 0) * INDICES_FACE_COUNT)}{
    TileBlock tile_block{block_type};

    auto face_v_it = local_vertex_positions.begin();
    auto face_uvs_it = uvs.begin();
    auto face_nrm_it = normals.begin();

    for(auto face_ind_it = indices.begin() ; face_ind_it != indices.end() ; ++face_ind_it){
        // obtain index of face
        int face_index = face_ind_it - indices.begin();
        // test if face is visible
        if (!visible_faces[face_index]) {
            continue;
        }
        //obtain arrays of actual face elements
        const auto& face_vertices = *(face_v_it++);
        const glm::vec3& face_normal = *(face_nrm_it++);
        const auto& face_uvs = *(face_uvs_it++);
        // iterate through actual face indices
        for(int i : *face_ind_it) {
            auto& actual_vertex = *(vertices_it++);
            // initialize texture coordinates
            float du = static_cast<float>(tile_block.face_tile(face_index) % 16) * S;
            float dv = static_cast<float>(tile_block.face_tile(face_index) / 16) * S;
            // obtain local(cube or flower) coordinates
            actual_vertex.position = center_position +
                    glm::rotateY(N * face_vertices[i], glm::radians(asy_rotation));
            // obtain world coordinates (first rotation and then translation)
            /*actual_vertex.position = center_position + glm::rotateY(actual_vertex.position,
                                                                    glm::radians(asy_rotation));*/


            // assign normal
            actual_vertex.normal = face_normal;
            actual_vertex.uv = {
                    du + (face_uvs[i].x ? B : A),
                    dv + (face_uvs[i].y ? B : A)
            };
        }
    }
}

template<unsigned int n_faces>
cube_vertex_iterator_t CubicObject<n_faces>::get_end() const {
    return begin + n_vertices;
}

template<unsigned int n_faces>
glm::vec3 CubicObject<n_faces>::rotate_asy(const glm::vec3 &v, float angle_degrees) {
    return {
            glm::cos(angle_degrees) * v.x - glm::sin(angle_degrees) * v.z,
        v.y,
            glm::sin(angle_degrees) * v.x + glm::cos(angle_degrees) * v.z,
    };
}

Cube::Cube(const BlockType &block_type, const std::array<bool, 6> &visible_faces, const glm::vec3 &position,
           cube_vertex_iterator_t vertices_it) :
                super(block_type, visible_faces, position, 0, vertices_it)
           {}

template<unsigned n_faces>
void CubicObject<n_faces>::print_vertex_info() {
    int index = 0;
    for(auto it = get_begin() ; it != get_end() ; ++it){
        fmt::print("pos: ({},{},{}), face: {}, normal: ({},{},{})\n", it->position.x, it->position.y, it->position.z,
                   (index++) / INDICES_FACE_COUNT, it->normal.x, it->normal.y, it->normal.z);
    }
}

template<>
const CubicObject<6>::PositionsMatrix CubicObject<6>::local_vertex_positions{{
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
const CubicObject<6>::NormalMatrix CubicObject<6>::normals{{
    {-1, 0, 0},
    {+1, 0, 0},
    {0, +1, 0},
    {0, -1, 0},
    {0, 0, -1},
    {0, 0, +1}
}};

template<>
const CubicObject<6>::IndicesMatrix CubicObject<6>::indices{{
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3}
}};

template<>
const CubicObject<6>::UvsMatrix CubicObject<6>::uvs{{
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}},
    {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}},
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}}
}};

template<unsigned int n_faces>
cube_vertex_iterator_t CubicObject<n_faces>::get_begin() const {
    return begin;
}

template<>
const CubicObject<4>::PositionsMatrix CubicObject<4>::local_vertex_positions{{
    {{{ 0, -1, -1}, { 0, -1, +1}, { 0, +1, -1}, { 0, +1, +1}}},
    {{{ 0, -1, -1}, { 0, -1, +1}, { 0, +1, -1}, { 0, +1, +1}}},
    {{{-1, -1,  0}, {-1, +1,  0}, {+1, -1,  0}, {+1, +1,  0}}},
    {{{-1, -1,  0}, {-1, +1,  0}, {+1, -1,  0}, {+1, +1,  0}}}
}};

template<>
const CubicObject<4>::NormalMatrix CubicObject<4>::normals{{
    {-1, 0, 0},
    {+1, 0, 0},
    {0, 0, -1},
    {0, 0, +1}
}};

template<>
const CubicObject<4>::IndicesMatrix CubicObject<4>::indices{{
    {-1, 0, 0},
    {+1, 0, 0},
    {0, 0, -1},
    {0, 0, +1}
}};

template<>
const CubicObject<4>::UvsMatrix CubicObject<4>::uvs{{
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}},
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}}
}};

template class CubicObject<6>;
template class CubicObject<4>;
