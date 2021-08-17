//
// Created by ultimatenick on 17/08/21.
//

#include <geometric.hpp>
#include "frustum.hpp"

float Plane::distance(const glm::vec3 &other_point) const {
    auto v = other_point - point;
    return glm::length(v * normal);
}

bool Frustum::is_inside(const glm::vec3 point) {
    for(const Plane& plane : planes){
        if(plane.distance(point) < 0){
            return false;
        }
    }
    return true;
}
