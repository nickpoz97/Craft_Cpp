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

template<int n_faces, int n_vertices_face>
void CubicObject<n_faces, n_vertices_face>::occlusion(CubicObject::IlluminationVector neighbors,
                                                      CubicObject::IlluminationVector lights,
                                                      CubicObject::IlluminationVector shades) {
    static const std::array<std::array<glm::vec3, 4>, 6> lookup3{{
        {{{0, 1, 3}, {2, 1, 5}, {6, 3, 7}, {8, 5, 7}}},
        {{{18, 19, 21}, {20, 19, 23}, {24, 21, 25}, {26, 23, 25}}},
        {{{6, 7, 15}, {8, 7, 17}, {24, 15, 25}, {26, 17, 25}}},
        {{{0, 1, 9}, {2, 1, 11}, {18, 9, 19}, {20, 11, 19}}},
        {{{0, 3, 9}, {6, 3, 15}, {18, 9, 21}, {24, 15, 21}}},
        {{{2, 5, 11}, {8, 5, 17}, {20, 11, 23}, {26, 17, 23}}}
    }};

    static const static const std::array<std::array<glm::vec4, 4>, 6> lookup4{{
        {{{0, 1, 3, 4}, {1, 2, 4, 5}, {3, 4, 6, 7}, {4, 5, 7, 8}}},
        {{{18, 19, 21, 22}, {19, 20, 22, 23}, {21, 22, 24, 25}, {22, 23, 25, 26}}},
        {{{6, 7, 15, 16}, {7, 8, 16, 17}, {15, 16, 24, 25}, {16, 17, 25, 26}}},
        {{{0, 1, 9, 10}, {1, 2, 10, 11}, {9, 10, 18, 19}, {10, 11, 19, 20}}},
        {{{0, 3, 9, 12}, {3, 6, 12, 15}, {9, 12, 18, 21}, {12, 15, 21, 24}}},
        {{{2, 5, 11, 14}, {5, 8, 14, 17}, {11, 14, 20, 23}, {14, 17, 23, 26}}}
    }};

    static const std::array<float,4> curve{0.0, 0.25, 0.5, 0.75};

    for(int i = 0 ; i < lookup3.size() ; i++){
        for(int j = 0 ; j < lookup3[0].size() ; j++){
            int corner = neighbors[lookup3[i][j].x];
            bool side1 = neighbors[lookup3[i][j].y];
            bool side2 = neighbors[lookup3[i][j].z];
            int value = (side1 && side2) ? 3 : corner + side1 + side2;
            float shade_sum = 0;
            float light_sum = 0;

            bool is_light = (lights[13] == 15);

            for (int k = 0; k < 4; k++) {
                shade_sum += shades[lookup4[i][j][k]];
                light_sum += lights[lookup4[i][j][k]];
            }

            if (is_light) {
                light_sum = 15 * 4 * 10;
            }
            float total = curve[value] + shade_sum / 4.0;
            ao[i][j] = glm::min(total, 1.0f);
            light[i][j] = light_sum / 15.0 / 4.0;
        }
    }
}
