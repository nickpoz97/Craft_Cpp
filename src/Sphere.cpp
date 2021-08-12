//
// Created by ultimatenick on 08/08/21.
//

#include <cmath>

#include "Sphere.hpp"

decltype(Sphere::triangles_indices) Sphere::triangles_indices{{
    {4, 3, 0}, {1, 4, 0},
    {3, 4, 5}, {4, 1, 5},
    {0, 3, 2}, {0, 2, 1},
    {5, 2, 3}, {5, 1, 2}
}};
decltype(Sphere::positions) Sphere::positions{{
    { 0, 0,-1}, { 1, 0, 0},
    { 0,-1, 0}, {-1, 0, 0},
    { 0, 1, 0}, { 0, 0, 1}
}};
decltype(Sphere::uvs) Sphere::uvs{{
        {0, 0.5}, {0, 0.5},
        {0, 0}, {0, 0.5},
        {0, 1}, {0, 0.5}
}};

Sphere::Sphere(float r, int detail) : vertices(8 * pow(4, detail) * 24), r{r}{
    for(const auto& ti : triangles_indices){
        const std::array<glm::vec3, 3> actual_pos {positions[ti[0]], positions[ti[1]], positions[ti[2]]};
        const std::array<glm::vec2, 3> actual_uvs {uvs[ti[0]], uvs[ti[1]], uvs[ti[2]]};

        rec_gen_sphere(detail, actual_pos, actual_uvs, vertices.begin());
    }
}

void Sphere::rec_gen_sphere(int detail, const std::array<glm::vec3, 3> &actual_pos,
                            const std::array<glm::vec2, 3> &actual_uvs,
                            std::vector<SphereVertex>::iterator &&it) {
    if(detail == 0){
        store_vertices(actual_pos, actual_uvs, it);
    }
    else{
        const auto& a{actual_pos[0]};
        const auto& b{actual_pos[1]};
        const auto& c{actual_pos[2]};

        const auto& uv_a{actual_uvs[0]};
        const auto& uv_b{actual_uvs[1]};
        const auto& uv_c{actual_uvs[2]};

        auto ab{0.5f * (a + b)};
        auto ac{0.5f * (a + c)};
        auto bc{0.5f * (b + c)};

        auto uv_ab{glm::vec2{0, 1 - acosf(ab.y) / PI}};
        auto uv_ac{glm::vec2{0, 1 - acosf(ac.y) / PI}};
        auto uv_bc{glm::vec2{0, 1 - acosf(bc.y) / PI}};

        rec_gen_sphere(detail - 1, {a, ab, ac}, {uv_a, uv_ab, uv_ac}, std::vector<SphereVertex>::iterator());
        rec_gen_sphere(detail - 1, {b, bc, ab}, {uv_b, uv_bc, uv_ab}, std::vector<SphereVertex>::iterator());
        rec_gen_sphere(detail - 1, {c, ac, bc}, {uv_c, uv_ac, uv_bc}, std::vector<SphereVertex>::iterator());
        rec_gen_sphere(detail - 1, {ab, bc, ac}, {uv_ab, uv_bc, uv_ac}, std::vector<SphereVertex>::iterator());
    }
}

void Sphere::store_vertices(const std::array<glm::vec3, 3> &actual_pos, const std::array<glm::vec2, 3> &actual_uvs,
                            std::vector<SphereVertex>::iterator &it) {

    auto pos_it {actual_pos.begin()};
    auto uvs_it {actual_uvs.begin()};

    for(;
        pos_it != actual_pos.end();
        ++pos_it, ++uvs_it
    ){
        glm::vec4 final_pos{r * (*pos_it), 1};
        *it = {final_pos, *uvs_it};
        ++it;
    }
}
