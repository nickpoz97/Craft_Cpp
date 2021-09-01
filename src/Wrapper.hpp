//
// Created by ultimatenick on 01/09/21.
//

#ifndef CPP_WRAPPER_HPP
#define CPP_WRAPPER_HPP


#include "glad/glad.h"
#include "Shader.hpp"
#include <string>

template<typename WrapperType>
struct value_type;

struct AttributesWrapper;
struct UniformsWrapper;

template<>
struct value_type<UniformsWrapper>{
    using t = std::pair<GLuint, std::string_view>;
};

template<>
struct value_type<AttributesWrapper>{
    using t = GLuint;
};

template <class WrapperType>
class Wrapper {
public:
    using value_type = typename value_type<WrapperType>::t;

    class Iterator : std::iterator<std::output_iterator_tag, value_type>{
    public:
        Iterator(const value_type* actual_attribute);
        value_type& operator*();
        value_type* operator->();
        Iterator& operator++();
        friend bool operator==(const Iterator& a, const Iterator& b);
        friend bool operator!=(const Iterator& a, const Iterator& b);
    private:
        value_type* actual_attribute;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    size_t get_size() const;
};

struct AttributesWrapper : public Wrapper<AttributesWrapper> {
    const value_type position{};
    const value_type normal{};
    const value_type uv{};
    const value_type model_matrix{};
};

struct UniformsWrapper : public Wrapper<UniformsWrapper>{
    const value_type sampler{0, "sampler"};
    const value_type camera{0, "camera"};
    const value_type timer{0, "timer"};
    const value_type extra1{0, "extra1"};
    const value_type extra2{0, "extra2"};
    const value_type extra3{0, "extra3"};
    const value_type extra4{0, "extra4"};
    const value_type viewproj_matrix{0, "viewproj_matrix"};
};

#endif //CPP_WRAPPER_HPP
