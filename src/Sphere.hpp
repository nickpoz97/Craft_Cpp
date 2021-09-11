//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP

#include <array>
#include <vector>

#include "vec3.hpp"
#include "vec2.hpp"
#include "Vertex.hpp"
#include "OpenglBuffer.hpp"

class Sphere{
private:
    static constexpr int N_TRIANGLES = 8;

    static std::array<glm::vec3,N_TRIANGLES> triangles_indices;
    static std::array<glm::vec3,6> positions;
    static std::array<glm::vec2,6> uvs;

    const float r;
    const OpenglBuffer<Uv3DVertex> gpu_buffer;

    using iterator_type = std::vector<Uv3DVertex>::iterator;
    iterator_type rec_gen_sphere(int detail, const std::array<glm::vec3, 3> &actual_pos_triangle, const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                 iterator_type it);
    iterator_type store_vertices(const std::array<glm::vec3, 3> &actual_pos_triangle, const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                 iterator_type it);
public:
    Sphere(float r, int detail);
    void render() const;
};


#endif //CPP_SPHERE_HPP
