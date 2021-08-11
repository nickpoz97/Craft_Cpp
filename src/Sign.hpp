//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_SIGN_HPP
#define CPP_SIGN_HPP

#include <string_view>
#include <vec3.hpp>

class Sign {
    glm::vec3 coordinate;
    int face;
    std::string_view text;
};


#endif //CPP_SIGN_HPP
