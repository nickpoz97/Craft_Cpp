//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_TEXT_HPP
#define CPP_TEXT_HPP


#include <string_view>
#include <vec2.hpp>
#include <list>
#include "Character.hpp"

class Text : public GameObject<Uv2DVertex>{
private:
    using SuperClass = GameObject<Uv2DVertex>;

    static std::vector<Uv2DVertex> gen_buffer(const glm::vec2& position, int n, std::string_view text);
public:
    Text(const glm::vec2& position, int n, std::string_view text);
    void render_object() const;
};


#endif //CPP_TEXT_HPP
