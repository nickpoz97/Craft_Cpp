//
// Created by ultimatenick on 07/08/21.
//

#include <vec3.hpp>
#include <numeric>
#include <ext/matrix_transform.hpp>
#include "CubicObject.hpp"
#include "costants.hpp"

template<unsigned n_faces>
CubicObject<n_faces>::CubicObject(const BlockType &block_type, const std::array<bool, 6> &visible_faces, const glm::mat4& transform,
                                  cube_vertex_iterator_t vertices_it) :
        begin{vertices_it},
        n_vertices{static_cast<size_t>(std::accumulate(visible_faces.begin(), visible_faces.end(), 0) * INDICES_FACE_COUNT)}{
    TileBlock tile_block{block_type};

    auto pos_it = local_vertex_positions.begin();
    auto uvs_it = uvs.begin();
    auto nrm_it = normals.begin();

    for(auto face_ind_it = indices.begin() ; face_ind_it != indices.end() ; ++face_ind_it){
        int face_index = face_ind_it - indices.begin();
        for(int face : *face_ind_it) {
            float du = (tile_block.face_tile(face_index) % 16) * S;
            float dv = (tile_block.face_tile(face_index) / 16) * S;

            if (!visible_faces[face_index]) {
                continue;
            }

            for (int i : *face_ind_it) {
                vertices_it->position = N * (*pos_it++)[i];
                vertices_it->normal = (*nrm_it++)[i];
                vertices_it->uv = {
                        du + (uvs_it->x ? B : A),
                        dv + (uvs_it->y ? B : A)
                };
                vertices_it->transform = transform;
                ++uvs_it;
                ++vertices_it;
            }
        }
    }
}

template<unsigned int n_faces>
cube_vertex_iterator_t CubicObject<n_faces>::get_end() const {
    return begin + n_vertices;
}

template<unsigned int n_faces>
glm::mat4 CubicObject<n_faces>::get_transform_matrix(const glm::vec3& position) {
    glm::mat4 transform{1.0f};
    transform = glm::translate(transform, position);
    return transform;
}

template<unsigned int n_faces>
glm::mat4 CubicObject<n_faces>::get_transform_matrix(const glm::vec3 &position, float rotation) {
    return glm::rotate(get_transform_matrix(position), glm::radians(rotation), {0,1,0});
}

Plant::Plant(const BlockType &block_type, const std::array<bool, 6> &visible_faces, const glm::vec3 &position,
             float rotation, cube_vertex_iterator_t vertices_it) :
                super(block_type, visible_faces, get_transform_matrix(position, rotation), vertices_it)
             {}


Cube::Cube(const BlockType &block_type, const std::array<bool, 6> &visible_faces, const glm::vec3 &position,
           cube_vertex_iterator_t vertices_it) :
                super(block_type, visible_faces, get_transform_matrix(position), vertices_it)
           {}

