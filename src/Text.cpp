//
// Created by ultimatenick on 13/08/21.
//

#include "Text.hpp"
#include "Character.hpp"

Text::Text(const glm::vec2& position, int n, const std::string_view text) {
    glm::vec2 char_pos = position;

    for(char c : text){
        Character(char_pos, n, n/2, c);
        char_pos.x += n;
    }
}
