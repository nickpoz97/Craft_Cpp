//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP


#include <string_view>
#include "glad/glad.h"
#include "AttributesWrapper.hpp"

class Shader {
private:
    std::string_view vs;
    std::string_view fs;
    const AttributesWrapper& attrib;
public:
    const AttributesWrapper &getAttrib() const;

private:

    Shader(std::string_view vs, std::string_view fs, const AttributesWrapper& attrib);
};


#endif //CPP_SHADER_HPP
