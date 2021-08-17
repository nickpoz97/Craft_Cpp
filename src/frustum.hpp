//
// Created by ultimatenick on 17/08/21.
//

#ifndef CPP_FRUSTUM_HPP
#define CPP_FRUSTUM_HPP


#include <vec3.hpp>
#include <array>

class Plane {
    glm::vec3 point;
    glm::vec3 normal;

public:
    float distance(const glm::vec3& other_point) const;
};

class Frustum {
    std::array<Plane, 6> planes;

    bool is_inside(const glm::vec3 point);
};

#endif //CPP_FRUSTUM_HPP
