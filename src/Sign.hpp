//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_SIGN_HPP
#define CPP_SIGN_HPP

#include <string_view>
#include <vec3.hpp>
#include "Vertex.hpp"
#include "Cube.hpp"
#include "Character3D.hpp"

class Sign {
    const glm::vec3& coordinate;
    Face face;
    std::string_view text;

    std::vector<Character3D> chars;
public:
    Sign(const glm::vec3& coordinate, Face face, const std::string_view text);
};


#endif //CPP_SIGN_HPP
