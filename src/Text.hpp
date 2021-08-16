//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_TEXT_HPP
#define CPP_TEXT_HPP


#include <string_view>
#include <vec2.hpp>
#include <list>
#include "Character.hpp"

class Text {
private:
    std::vector<Character> chars;
public:
    Text(const glm::vec2& position, int n, const std::string_view text);
    void draw();
};


#endif //CPP_TEXT_HPP
