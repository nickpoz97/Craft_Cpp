//
// Created by ultimatenick on 10/08/21.
//

#include "Sign.hpp"
#include "string"
#include "sstream"

const std::array<int,8> Sign::glyph_dx{0, 0, -1, 1, 1, 0, -1, 0};
const std::array<int,8> glyph_dz{1, -1, 0, 0, 0, -1, 0, 1};

const std::array<int,8> line_dx{0, 0, 0, 0, 0, 1, 0, -1};
const std::array<int,8> line_dy{-1, -1, -1, -1, 0, 0, 0, 0};
const std::array<int,8> line_dz{0, 0, 0, 0, 1, 0, -1, 0};

Sign::Sign(const glm::vec3 &coordinate, Face face, std::string_view text) : coordinate{coordinate}, face{face}, sign_text{text} , chars(){

    int rows = wrap_text();
    std::istringstream lss{sign_text};

    std::string line;
    std::string token;

    glm::vec3 d{glyph_dx[static_cast<int>(face)], 0, glyph_dz[static_cast<int>(face)]};
    glm::vec3 ld{line_dx[static_cast<int>(face)], line_dy[static_cast<int>(face)], line_dz[static_cast<int>(face)]};

    float n = 1.0 / (max_width / 10);

    glm::vec3 s{coordinate - n * (rows-1) * (line_height/2) * ld};

    while (std::getline(lss, line)){
        int lw = line_width(line);
        lw = glm::min(lw, max_width);

        glm::vec3 r{s - d * static_cast<float>(lw) / static_cast<float>(max_width) / 2.0f};
        for(char c : line){
            int c_width = char_width(c);
            lw -= c_width;
            if(lw < 0){
                break;
            }
            r += d * static_cast<float>(lw) / static_cast<float>(max_width) / 2.0f;
            if(c != ' '){
                chars.push_back(Character3D(r, n/2, face, c));
            }
            r += d * static_cast<float>(lw) / static_cast<float>(max_width) / 2.0f;
        }
        s += n * line_height * ld;
        rows--;
        if(rows == 0){
            break;
        }
    }
}

// no support for '\n'
int Sign::wrap_text() {
    static_assert(max_line_length > 0);
    int n_lines = 0;

    int n_iterations = sign_text.length() / max_line_length;
    if(sign_text.length() % max_line_length != 0) {
        ++n_lines;
    }

    for(int i = n_iterations ; i > 0 ; --i){
        // find the first space from max_line_length to the beginning
        int n = sign_text.rfind(' ', i * max_line_length);
        if (n != std::string_view::npos) {
            sign_text.at(n) = '\n';
            ++n_lines;
        }
    }
    return n_lines;
}

int Sign::char_width(char c) {
    static const std::array<int, 128> lookup = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            4, 2, 4, 7, 6, 9, 7, 2, 3, 3, 4, 6, 3, 5, 2, 7,
            6, 3, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 5, 6, 5, 7,
            8, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 6, 5, 8, 8, 6,
            6, 7, 6, 6, 6, 6, 8,10, 8, 6, 6, 3, 6, 3, 6, 6,
            4, 7, 6, 6, 6, 6, 5, 6, 6, 2, 5, 5, 2, 9, 6, 6,
            6, 6, 6, 6, 5, 6, 6, 6, 6, 6, 6, 4, 2, 5, 7, 0
    };
    return lookup[c];
}

int Sign::line_width(std::string_view s) {
    int width = 0;
    for(char c : s){
        width += char_width(c);
    }
}

std::string_view Sign::get_text() const{
    return sign_text;
}

const std::vector<Character3D> &Sign::get_chars() const{
    return chars;
}
