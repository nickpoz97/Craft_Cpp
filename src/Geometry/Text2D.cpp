//
// Created by ultimatenick on 13/08/21.
//

#include "Geometry/Text2D.hpp"
#include "Geometry/Character.hpp"

namespace CraftCpp {
Text2D::Text2D(const glm::vec2 &position, float n, std::string_view text) : SuperClass{
        genBuffer(position, n, text)} {
}

void Text2D::renderObject() const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SuperClass::renderObject();

    glDisable(GL_BLEND);
}

std::vector<Uv2DVertex> Text2D::genBuffer(const glm::vec2 &position, float n, std::string_view text) {
    std::vector<Uv2DVertex> buffer(text.size() * Character::getNVertices());
    auto buffer_it{buffer.begin()};

    glm::vec2 char_pos = position;

    for (char c: text) {
        Character character{char_pos, n, n / 2, c};
        buffer_it = std::copy(character.begin(), character.end(), buffer_it);
        char_pos.x += n;
    }
    return buffer;
}
}