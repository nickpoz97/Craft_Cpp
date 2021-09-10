//
// Created by ultimatenick on 01/09/21.
//

#ifndef CPP_WRAPPER_HPP
#define CPP_WRAPPER_HPP

#include <string>
#include <array>

#include "glad/glad.h"

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
    using t = std::pair<GLuint, bool>;
};

template <class WrapperType>
class Wrapper {
public:
    using value_type = typename value_type<WrapperType>::t;

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

    UniformsWrapper() = default;
    UniformsWrapper(std::string_view extra1_name, std::string_view extra2_name, std::string_view extra3_name, std::string_view extra4_name);
    UniformsWrapper(std::array<std::string_view, 4> extra_names);
};

#endif //CPP_WRAPPER_HPP
