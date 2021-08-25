//
// Created by ultimatenick on 07/08/21.
//

#include <vec3.hpp>
#include "CubicObject.hpp"
#include "costants.hpp"

template<unsigned n_faces>
CubicObject<n_faces>::CubicObject(const Item& item, const TileBlock& visible_faces, const glm::mat4& transform) :
SuperClass(visible_faces.count_visible_faces() * INDICES_FACE_COUNT) {
    Block tiles{item};

    auto pos_it = local_vertex_positions.begin();
    auto uvs_it = uvs.begin();
    auto nrm_it = normals.begin();
    auto vertices_it = vertices.begin();

    for(const auto& face_ind_it : indices){
        int face_index = face_ind_it - indices.begin();
        float du = (tiles.face_val(face_index) % 16) * S;
        float dv = (tiles.face_val(face_index) / 16) * S;

        if(visible_faces.get_face_val(face_index)){
            continue;
        }

        for(int i : face_ind_it){
            vertices_it->position = N * (*pos_it++)[i];
            vertices_it->normal = (*nrm_it++)[i];
            vertices_it->uv = {
                du + (uvs_it->x ? B : A),
                dv + (uvs_it->y ? B : A)
            };
            ++uvs_it;
            ++vertices_it;
        }
    }
    apply_transform(transform);
}