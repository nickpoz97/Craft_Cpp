//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP


#include <string_view>
#include "glad/glad.h"
#include "Attrib.hpp"

class Shader {
private:
    std::string_view vs;
    std::string_view fs;
    const Attrib& attrib;
public:
    const Attrib &getAttrib() const;

private:

    Shader(std::string_view vs, std::string_view fs, const Attrib& attrib);
};


#endif //CPP_SHADER_HPP
