//
// Created by ultimatenick on 07/08/21.
//

#include <vec3.hpp>
#include "CubicObject.hpp"

using CubeObject =  CubicObject<6,VERTICES_FACE_COUNT>;
using PlantObject = CubicObject<4,VERTICES_FACE_COUNT>;

template<int n_faces, int n_vertices_face>
CubicObject<n_faces,n_vertices_face>::CubicObject(
            const TileBlock& tiles,
            const LightMatrix& ao,
            const LightMatrix& light,
            const PositionsMatrix& local_vertex_positions,
            const IndicesMatrix& indices,
            const NormalMatrix& normals,
            const UvsMatrix& uvs,
            float s,
            float a,
            float b
        ) : SuperClass() {
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
            v_it->light = light_face_it[*ind_it];
        }
    }

    this->set_buffer_data();
}
