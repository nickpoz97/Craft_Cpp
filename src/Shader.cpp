//
// Created by ultimatenick on 11/08/21.
//

#include "Shader.hpp"

Shader::Shader(const std::string_view vs, const std::string_view fs, const Attrib &attrib) :
    vs{vs}, fs{fs}, attrib{attrib} {}

const Attrib &Shader::getAttrib() const {
    return attrib;
}
