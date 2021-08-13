//
// Created by ultimatenick on 29/07/21.
//

#include "Cube.hpp"
#include "Block.hpp"
#include "vec4.hpp"

#include "CubicObject.hpp"

Cube::Cube(const TileBlock& tiles, const LightMatrix&  ao, const LightMatrix&  light) : SuperClass{tiles, ao, light, local_vertex_positions,
                                                                                                   true_indices(ao), normals, uvs, s ,a, b} {

}

decltype(Cube::indices) Cube::true_indices(const CubicObject::LightMatrix &ao) {
    decltype(indices) t_indices{};

    auto ao_face {ao.begin()};
    auto i_face {t_indices.begin()};

    for(; ao_face != ao.end() ; ++ao_face, ++i_face){
        int face_index = i_face - t_indices.begin();
        bool cond = (*ao_face)[0] + (*ao_face)[3] > (*ao_face)[1] + (*ao_face)[2];
        *i_face = cond ? flipped[face_index] : indices[face_index];
    }

    return t_indices;
}
