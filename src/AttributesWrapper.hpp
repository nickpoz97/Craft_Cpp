//
// Created by ultimatenick on 14/08/21.
//

#ifndef CPP_ATTRIBUTESWRAPPER_HPP
#define CPP_ATTRIBUTESWRAPPER_HPP


#include "glad/glad.h"
#include "Shader.hpp"

struct AttributesWrapper {
    using value_type = GLuint;

    const GLuint program;
    const GLuint position;
    const GLuint normal;
    const GLuint uv;
    const GLuint matrix;
    const GLuint sampler;
    const GLuint camera;
    const GLuint timer;
    const GLuint extra1;
    const GLuint extra2;
    const GLuint extra3;
    const GLuint extra4;

    class Iterator : std::iterator<std::output_iterator_tag, value_type>{
    public:
        Iterator(const value_type* actual_attribute);
        const value_type& operator*();
        const value_type* operator->();
        Iterator& operator++();
        friend bool operator==(const Iterator& a, const Iterator& b);
        friend bool operator!=(const Iterator& a, const Iterator& b);
    private:
        const value_type* actual_attribute;
    };

    Iterator begin() const;
    Iterator end() const;
};

#endif //CPP_ATTRIBUTESWRAPPER_HPP
