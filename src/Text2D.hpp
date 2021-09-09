//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_TEXT2D_HPP
#define CPP_TEXT2D_HPP


#include <string_view>
#include <vec2.hpp>
#include <list>
#include "Character.hpp"

class Text2D : public GameObject<Uv2DVertex>{
private:
    using SuperClass = GameObject<Uv2DVertex>;

    static std::vector<Uv2DVertex> gen_buffer(const glm::vec2& position, int n, std::string_view text);
public:
    Text2D(const glm::vec2& position, float n, std::string_view text);
};


#endif //CPP_TEXT2D_HPP
