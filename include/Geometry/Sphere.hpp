//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP

#include <array>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/OpenglBuffer.hpp"
#include "Geometry/RenderableEntity.hpp"

// TODO extend RenderableEntity with Sphere
class Sphere : public RenderableEntity<Uv3DVertex>{
private:
    using SuperClass = RenderableEntity<Uv3DVertex>;
    static constexpr int N_TRIANGLES = 8;

    static std::array<glm::vec3,N_TRIANGLES> triangles_indices;
    static std::array<glm::vec3,6> positions;
    static std::array<glm::vec2,6> uvs;

    const float r;

    using iterator_type = std::vector<Uv3DVertex>::iterator;
    iterator_type rec_gen_sphere(int detail, const std::array<glm::vec3, 3> &actual_pos_triangle, const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                 iterator_type it);
    iterator_type fill_local_buffer(const std::array<glm::vec3, 3> &actual_pos_triangle, const std::array<glm::vec2, 3> &actual_uvs_triangle,
                                    iterator_type it) const;
public:
    Sphere(float r, int detail);
};


#endif //CPP_SPHERE_HPP
