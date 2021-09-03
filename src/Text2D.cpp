//
// Created by ultimatenick on 13/08/21.
//

#include "Text2D.hpp"
#include "Character.hpp"

Text2D::Text2D(const glm::vec2& position, int n, std::string_view text) : SuperClass{gen_buffer(position, n, text)} {
}

void Text2D::render_object() const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SuperClass::render_object();

    glDisable(GL_BLEND);
}

std::vector<Uv2DVertex> Text2D::gen_buffer(const glm::vec2 &position, int n, std::string_view text) {
    std::vector<Uv2DVertex> buffer(text.size());
    auto buffer_it{buffer.begin()};

    glm::vec2 char_pos = position;

    for(char c : text){
        Character character{char_pos, n, n/2, c};
        buffer_it = std::copy(character.begin(), character.end(), buffer_it);
        char_pos.x += n;
    }
    return buffer;
}
