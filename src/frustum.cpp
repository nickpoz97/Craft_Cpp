//
// Created by ultimatenick on 17/08/21.
//

#include "geometric.hpp"
#include "frustum.hpp"
#include "trigonometric.hpp"

float Plane::distance(const glm::vec3 &other_point) const {
    auto v = other_point - point;
    return glm::dot(v, normal);
}

Plane::Plane(const glm::vec3& point, const glm::vec3 normal) : point{point}, normal{normal}{}

bool Frustum::is_inside(const glm::vec3& point) const{
    for(const Plane& plane : planes){
        if(plane.distance(point) < 0){
            return false;
        }
    }
    return true;
}

SidePoints Frustum::compute_sidepoints(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                                       const glm::vec3 &view_dir, float width, float height, float dist) {
    return {
        view_dir * view_pos + view_dir * dist + up * (height / 2),
        view_pos + view_dir * dist - up * (height / 2),
        view_pos + view_dir * dist + right * (width / 2),
        view_pos + view_dir * dist - right * (width / 2)
    };
}

void Frustum::update(bool ortho) {
    const auto& up = player.get_up_vector();
    const auto& right = player.get_right_vector();
    const auto& view_pos = player.get_position();
    const auto& view_dir = player.get_camera_direction_vector();

    auto center_sidepoints{compute_sidepoints(
            up, right, view_pos, view_dir,
            (w_far + w_near) / 2,
            (h_far + h_near) / 2,
            near_dist + (far_dist - near_dist) / 2)
    };

    if(ortho)
        set_frustum_ortho(up, right, view_pos, view_dir, center_sidepoints);
    else
        set_frustum_persp(up, right, view_pos, view_dir, center_sidepoints);
}

// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
void Frustum::set_frustum_persp(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                                const glm::vec3 &view_dir, const SidePoints& center_sidepoints) {
    auto ppv = center_sidepoints - view_pos;

    planes = std::array<Plane, 6>{{
            {view_dir * near_dist, view_dir},                   // near
            {view_dir * far_dist, -view_dir},                   // far
            {center_sidepoints.r,glm::cross(ppv.r, up)},        // right plane normal
            {center_sidepoints.l,glm::cross(up, ppv.l)},        // left plane normal
            {center_sidepoints.t,glm::cross(ppv.t, right)},     // top plane normal
            {center_sidepoints.b,glm::cross(right, ppv.b)}      // bottom plane normal
    }};
}

int Frustum::set_frustum_ortho(const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &view_pos,
                               const glm::vec3 &view_dir, const SidePoints &center_sidepoints) {
    planes = std::array<Plane, 6>{{
            {view_dir * near_dist, view_dir},                   // near
            {view_dir * far_dist, -view_dir},                   // far
            {center_sidepoints.r, -right},                            // right plane normal
            {center_sidepoints.l, right},                             // left plane normal
            {center_sidepoints.t, -up},                               // top plane normal
            {center_sidepoints.b, up}                                 // bottom plane normal
    }};
}

/*Frustum::Frustum(float w_far, float w_near, float h_far, float h_near, float near_dist, float far_dist) :
    w_far{w_far},
    w_near{w_near},
    h_far{h_far},
    h_near{h_near},
    near_dist{near_dist},
    far_dist{far_dist}
{}*/

Frustum::Frustum(float fov_degrees, float near_dist, float far_dist, float ratio, const Player& player) :
    near_dist{near_dist},
    far_dist{far_dist},
    h_near{2.0f * static_cast<float>(near_dist * glm::tan(glm::radians(fov_degrees) / 2))},
    h_far{2.0f * static_cast<float>(far_dist * glm::tan(glm::radians(fov_degrees) / 2))},
    w_near{h_near * ratio},
    w_far{h_far * ratio},
    player{player}
{}

SidePoints operator-(const SidePoints& s1, const SidePoints& s2){
    return {
        s1.t - s2.t,
        s1.b - s2.b,
        s1.r - s2.r,
        s1.l - s2.l
    };
}

SidePoints operator/(const SidePoints &s1, float c) {
    return {
        s1.t / c,
        s1.b / c,
        s1.r / c,
        s1.l / c
    };
}

SidePoints operator+(const SidePoints& s1, const SidePoints& s2) {
    return{
        s1.t + s2.t,
        s1.b + s2.b,
        s1.r + s2.r,
        s1.l + s2.l
    };
}

SidePoints operator-(const SidePoints &s1, const glm::vec3 &v) {
    return {
        s1.t - v,
        s1.b - v,
        s1.r - v,
        s1.l - v
    };
}
