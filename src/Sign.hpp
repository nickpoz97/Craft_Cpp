//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_SIGN_HPP
#define CPP_SIGN_HPP

#include <string_view>
#include <string>

#include "Vertex.hpp"
#include "Cube.hpp"
#include "Character3D.hpp"
#include "costants.hpp"
#include "vec3.hpp"

class Sign {
    static const std::array<int,8> glyph_dx;
    static const std::array<int,8> glyph_dz;
    static const std::array<int,8> line_dx;
    static const std::array<int,8> line_dy;
    static const std::array<int,8> line_dz;

    const glm::ivec3 coordinate;
    Face face;
    std::string sign_text;
    std::vector<Character3D> chars;
    static constexpr int max_width = MAX_SIGN_WIDTH;
    static constexpr float line_height = SIGN_LINE_HEIGHT;
    static constexpr int n_lines = N_SIGN_LINES;
    static constexpr int max_line_length = MAX_LINE_LENGTH;

    int wrap_text();
    static int char_width(char c);
    static int line_width(std::string_view s);
public:
    Sign(const glm::vec3& coordinate, Face face, std::string_view text);
    std::string_view get_text() const;
    const std::vector<Character3D>& get_chars() const;
};


#endif //CPP_SIGN_HPP
