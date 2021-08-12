//
// Created by ultimatenick on 07/08/21.
//

#include <vec3.hpp>
#include "CubicObject.hpp"

template<int n_faces, int n_vertices_face>
CubicObject<n_faces,n_vertices_face>::CubicObject(const TileBlock& tiles) : SuperClass() {
    auto v_it {(this->vertices).begin()};
    auto ind_face_it {indices.begin()};
    auto pos_face_it {local_vertex_positions.begin()};
    auto ao_face_it {ao.begin()};
    auto light_face_it {light.begin()};
    auto uvs_face_it {uvs.begin()};

    for(; ind_face_it != indices.end() ; ++ind_face_it, ++pos_face_it, ++uvs_face_it, ++ao_face_it, ++ao_face_it){
        int face_index = ind_face_it - indices.begin();

        auto ind_it = get_face_indices(face_index).begin();
        auto pos_it = pos_face_it->begin();
        auto uvs_it = uvs_face_it->begin();

        for(; ind_it != ind_it.end() ; ++v_it, ++pos_it, ++ind_it, ++uvs_it){
            v_it->face = static_cast<Face>(face_index);
            v_it->position = N * (*pos_it);
            v_it->index = *ind_it;
            v_it->uv = {
                static_cast<float>(tiles[face_index] % 16) * s + (*uvs_it.x ? b : a),
                static_cast<float>(tiles[face_index] / 16) * s + (*uvs_it.x ? b : a)
            };
            v_it->ao = ao_face_it[*ind_it];
            v_it->light = ao_face_it[*ind_it];
        }
    }
}

template<int n_faces, int n_vertices_face>
void CubicObject<n_faces, n_vertices_face>::apply_transform(const glm::mat4 &transform) {
    for(auto& v : this->vertices){
        v.position = transform * v.position;
    }
}

template<>
std::array<int, VERTICES_FACE_COUNT> CubicObject<6, VERTICES_FACE_COUNT>::get_face_indices(int face_index,
                                                                                         std::array<float, 4> ao_face) {
    decltype(indices) flipped{{
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1},
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1},
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1}
    }};

    return (ao_face[0] + ao_face[3] > ao_face[1] + ao_face[2]) ? flipped[face_index] : indices[face_index];
}

template<>
std::array<int, VERTICES_FACE_COUNT> CubicObject<4, VERTICES_FACE_COUNT>::get_face_indices(int face_index,
                                                                                           std::array<float, 4> ao_face){
    return indices[face_index];
}