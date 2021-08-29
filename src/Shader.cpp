//
// Created by ultimatenick on 11/08/21.
//

#include "Shader.hpp"

Shader::Shader(const std::string_view vs, const std::string_view fs, const AttributesWrapper &attrib) :
    vs{vs}, fs{fs}, attrib{attrib} {}

const AttributesWrapper &Shader::getAttrib() const {
    return attrib;
}
