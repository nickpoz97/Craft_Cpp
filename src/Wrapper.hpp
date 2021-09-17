//
// Created by ultimatenick on 01/09/21.
//

#ifndef CPP_WRAPPER_HPP
#define CPP_WRAPPER_HPP

#include <string>
#include <array>

#include "glad/glad.h"

template<typename WrapperType>
struct wrapper_value;

struct AttributesWrapper;
struct UniformsWrapper;

template<>
struct wrapper_value<UniformsWrapper>{
    struct Uniform{
        GLuint id;
        std::string_view name;
    };

    using t = Uniform;
};

template<>
struct wrapper_value<AttributesWrapper>{
    using t = std::pair<GLuint, bool>;
};

template <class WrapperType>
class Wrapper {
public:
    using value_type = typename wrapper_value<WrapperType>::t;

    class Iterator : std::iterator<std::output_iterator_tag, value_type>{
    public:
        Iterator(value_type* actual_attribute);
        value_type& operator*();
        value_type* operator->();
        Iterator& operator++();
        bool operator==(const Iterator& b);
        bool operator!=(const Iterator& b);
    private:
        value_type* actual_attribute;
    };

    Iterator begin() ;
    Iterator end();

private:
    size_t get_size() const;
};

struct AttributesWrapper : public Wrapper<AttributesWrapper> {
    value_type position{};
    value_type normal{};
    value_type uv{};
    //value_type model_matrix{};
};

struct UniformsWrapper : public Wrapper<UniformsWrapper>{
    value_type sampler{0, "sampler"};
    value_type camera{0, "camera_pos"};
    value_type timer{0, "timer"};
    value_type viewproj_matrix{0, "viewproj_matrix"};
    value_type PI{0, "PI"};
};

#endif //CPP_WRAPPER_HPP
