//
// Created by ultimatenick on 15/08/21.
//

#include "Character3D.hpp"

const decltype(Character3D::local_positions) Character3D::local_positions = {{
    {{{0, -2, -1}, {0, +2, +1}, {0, +2, -1}, {0, -2, -1}, {0, -2, +1}, {0, +2, +1}}},
    {{{0, -2, -1}, {0, +2, +1}, {0, -2, +1}, {0, -2, -1}, {0, +2, -1}, {0, +2, +1}}},
    {{{-1, -2, 0}, {+1, +2, 0}, {+1, -2, 0}, {-1, -2, 0}, {-1, +2, 0}, {+1, +2, 0}}},
    {{{-1, -2, 0}, {+1, -2, 0}, {+1, +2, 0}, {-1, -2, 0}, {+1, +2, 0}, {-1, +2, 0}}},
    {{{-1, 0, +2}, {+1, 0, +2}, {+1, 0, -2}, {-1, 0, +2}, {+1, 0, -2}, {-1, 0, -2}}},
    {{{-2, 0, +1}, {+2, 0, -1}, {-2, 0, -1}, {-2, 0, +1}, {+2, 0, +1}, {+2, 0, -1}}},
    {{{+1, 0, +2}, {-1, 0, -2}, {-1, 0, +2}, {+1, 0, +2}, {+1, 0, -2}, {-1, 0, -2}}},
    {{{+2, 0, -1}, {-2, 0, +1}, {+2, 0, +1}, {+2, 0, -1}, {-2, 0, -1}, {-2, 0, +1}}}
}};

const decltype(Character3D::uvs) Character3D::uvs = {{
    {{{0, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 1}}},
    {{{1, 0}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1}}},
    {{{1, 0}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1}}},
    {{{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}}},
    {{{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}}},
    {{{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}}},
    {{{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}}},
    {{{0, 1}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}}}
}};

const decltype(Character3D::offsets) Character3D::offsets = {{
    {-1, 0, 0}, {+1, 0, 0}, {0, 0, -1}, {0, 0, +1},
    {0, +1, 0}, {0, +1, 0}, {0, +1, 0}, {0, +1, 0},
}};

Character3D::Character3D(const glm::vec3 &position, Face face, float n, float m, char c){
    float pu = s / 5;
    float pv = s / 2.5;
    float u1 = pu;
    float v1 = pv;
    float u2 = s - pu;
    float v2 = s * 2 - pv;
    int w = c - 32;
    float du = (w % 16) * s;
    float dv = 1 - (w / 16 + 1) * s * 2;

    int face_index = static_cast<int>(face);
    glm::vec3 vertex_position{position + p * offsets[face_index]};

    auto pos_it = local_positions.begin();
    auto uv_it = uvs.begin();

    for(auto& v : vertices){
        v.position = vertex_position + n * (*pos_it++)[face_index];

        const glm::vec2& uv_bool = (*uv_it++)[face_index];
        v.uv = {du + (uv_bool.x ? u2 : u1), dv + (uv_bool.y ? v2 : v1)};
    }
}
