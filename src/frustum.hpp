//
// Created by ultimatenick on 17/08/21.
//

#ifndef CPP_FRUSTUM_HPP
#define CPP_FRUSTUM_HPP


#include <vec3.hpp>
#include <array>
#include "Player.hpp"

struct SidePoints{
    glm::vec3 t;
    glm::vec3 b;
    glm::vec3 r;
    glm::vec3 l;

    friend SidePoints operator-(const SidePoints& s1, const SidePoints& s2);
    friend SidePoints operator-(const SidePoints& s1, const glm::vec3& v);
    friend SidePoints operator/(const SidePoints& s1, float c);
    friend SidePoints operator+(const SidePoints& s1, const SidePoints& s2);
};

class Plane {
    glm::vec3 point;
    glm::vec3 normal;

public:
    Plane() = default;
    Plane(const glm::vec3& point, const glm::vec3 normal);
    float distance(const glm::vec3& other_point) const;
};

class Frustum {
    float w_far, w_near, h_far, h_near, near_dist, far_dist;

    std::array<Plane,6> planes;
    static SidePoints compute_sidepoints(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                                         const glm::vec3& view_dir,
                                         float width, float height, float dist);
public:
    Frustum(float w_far, float w_near, float h_far, float h_near, float near_dist, float far_dist;);
    void update(const Player& player, bool ortho);
    bool is_inside(const glm::vec3& point) const;

    void set_frustum_persp(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                      const glm::vec3 &view_dir, const SidePoints &center_sidepoints);
    void set_frustum_ortho(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                      const glm::vec3 &view_dir, const SidePoints &center_sidepoints)
};

#endif //CPP_FRUSTUM_HPP
