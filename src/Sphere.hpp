//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP

#include <array>
#include <vector>

#include "CubicObject.hpp"

//TODO update mathematics
class Sphere{
private:
    static constexpr int N_TRIANGLES = 8;

    static std::array<glm::vec3,N_TRIANGLES> triangles_indices;
    static std::array<glm::vec3,6> positions;
    static std::array<glm::vec2,6> uvs;
    static constexpr float PI = static_cast<float>(3.14159265359);

    std::vector<SphereVertex> vertices;
    const float r;

    void
    rec_gen_sphere(int detail, const std::array<glm::vec3, 3> &actual_pos, const std::array<glm::vec2, 3> &actual_uvs,
                   std::vector<SphereVertex>::iterator &&it);
    void store_vertices(const std::array<glm::vec3, 3> &actual_pos, const std::array<glm::vec2, 3> &actual_uvs,
                        std::vector<SphereVertex>::iterator &it);

public:
    Sphere(float r, int detail);
};


#endif //CPP_SPHERE_HPP
