//
// Created by ultimatenick on 13/08/21.
//

#include "Text.hpp"
#include "Character.hpp"

Text::Text(const glm::vec2& position, int n, const std::string_view text) : chars() {
    glm::vec2 char_pos = position;

    for(char c : text){
        chars.emplace_back(char_pos, n, n/2, c);
        char_pos.x += n;
    }
}

void Text::draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(Character c : chars){
        c.draw_triangles();
    }
    glDisable(GL_BLEND);
}
