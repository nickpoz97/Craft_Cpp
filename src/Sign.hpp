//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_SIGN_HPP
#define CPP_SIGN_HPP

#include <string_view>

#include "Vec.hpp"

class Sign {
    Vec<int,3> coordinate;
    int face;
    std::string_view text;
};


#endif //CPP_SIGN_HPP
