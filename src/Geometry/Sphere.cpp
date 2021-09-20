//
// Created by ultimatenick on 08/08/21.
//

#include <cmath>

#include "Sphere.hpp"
#include "ext/scalar_constants.hpp"

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

Sphere::Sphere(float r, int detail) : SuperClass{}, r{r}{
    std::vector<Uv3DVertex> vertices(8 * pow(4, detail) * 24);
    auto it = vertices.begin();

    for(const auto& ti : triangles_indices){
        const std::array<glm::vec3, 3> actual_pos {positions[ti[0]], positions[ti[1]], positions[ti[2]]};
        const std::array<glm::vec2, 3> actual_uvs {uvs[ti[0]], uvs[ti[1]], uvs[ti[2]]};

        it = rec_gen_sphere(detail, actual_pos, actual_uvs, it);
    }

    update_buffer(vertices);
}

Sphere::iterator_type Sphere::rec_gen_sphere(int detail, const std::array<glm::vec3, 3> &actual_pos_triangle,
                                             const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                             iterator_type it) {
    if(detail == 0){
        it = fill_local_buffer(actual_pos_triangle, actual_uvs_triangle, it);
        return it;
    }
    const auto& a{actual_pos_triangle[0]};
    const auto& b{actual_pos_triangle[1]};
    const auto& c{actual_pos_triangle[2]};

    const auto& uv_a{actual_uvs_triangle[0]};
    const auto& uv_b{actual_uvs_triangle[1]};
    const auto& uv_c{actual_uvs_triangle[2]};

    auto ab{0.5f * (a + b)};
    auto ac{0.5f * (a + c)};
    auto bc{0.5f * (b + c)};

    auto uv_ab{glm::vec2{0, 1 - acosf(ab.y) / glm::pi<float>()}};
    auto uv_ac{glm::vec2{0, 1 - acosf(ac.y) / glm::pi<float>()}};
    auto uv_bc{glm::vec2{0, 1 - acosf(bc.y) / glm::pi<float>()}};

    it = rec_gen_sphere(detail - 1, {a, ab, ac}, {uv_a, uv_ab, uv_ac}, it);
    it = rec_gen_sphere(detail - 1, {b, bc, ab}, {uv_b, uv_bc, uv_ab}, it);
    it = rec_gen_sphere(detail - 1, {c, ac, bc}, {uv_c, uv_ac, uv_bc}, it);
    it = rec_gen_sphere(detail - 1, {ab, bc, ac}, {uv_ab, uv_bc, uv_ac}, it);
    return it;
}

Sphere::iterator_type Sphere::fill_local_buffer(const std::array<glm::vec3, 3> &actual_pos_triangle, const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                                iterator_type it) const {

    auto pos_it {actual_pos_triangle.begin()};
    auto uvs_it {actual_uvs_triangle.begin()};

    for(; pos_it != actual_pos_triangle.end(); ++pos_it, ++uvs_it
    ){
        glm::vec3 final_pos{r * (*pos_it)};
        *(it++) = {final_pos, *uvs_it};
    }
    // next pos on buffer
    return it;
}
