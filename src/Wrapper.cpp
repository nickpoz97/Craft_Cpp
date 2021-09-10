//
// Created by ultimatenick on 01/09/21.
//

#include "Wrapper.hpp"

template<typename WrapperType>
Wrapper<WrapperType>::Iterator::Iterator(value_type* actual_attribute) : actual_attribute{actual_attribute} {}

template<typename WrapperType>
typename Wrapper<WrapperType>::value_type &Wrapper<WrapperType>::Iterator::operator*() {
    return *actual_attribute;
}

template<typename WrapperType>
typename Wrapper<WrapperType>::value_type *Wrapper<WrapperType>::Iterator::operator->() {
    return actual_attribute;
}

template<typename WrapperType>
typename Wrapper<WrapperType>::Iterator& Wrapper<WrapperType>::Iterator::operator++() {
    ++actual_attribute;
    return *this;
}

template<typename WrapperType>
bool Wrapper<WrapperType>::Iterator::operator==(const typename Wrapper<WrapperType>::Iterator &b) {
    return this->actual_attribute == b.actual_attribute;
}

template<typename WrapperType>
bool Wrapper<WrapperType>::Iterator::operator!=(const typename Wrapper<WrapperType>::Iterator &b) {
    return this->actual_attribute != b.actual_attribute;
}

template<typename WrapperType>
size_t Wrapper<WrapperType>::get_size() const{
    return sizeof(WrapperType);
}

template<typename WrapperType>
typename Wrapper<WrapperType>::Iterator Wrapper<WrapperType>::begin() {
    return {reinterpret_cast<value_type*>(this)};
}

template<typename WrapperType>
typename Wrapper<WrapperType>::Iterator Wrapper<WrapperType>::end() {
    return {reinterpret_cast<value_type*>(this) + (get_size() / sizeof(value_type))};
}

UniformsWrapper::UniformsWrapper(std::string_view extra1_name,
                                 std::string_view extra2_name,
                                 std::string_view extra3_name,
                                 std::string_view extra4_name) :
                                 extra1{0, extra1_name},
                                 extra2{0, extra1_name},
                                 extra3{0, extra1_name},
                                 extra4{0, extra1_name}
                                 {}

UniformsWrapper::UniformsWrapper(std::array<std::string_view, 4> extra_names) :
    UniformsWrapper{extra_names[0], extra_names[1], extra_names[2], extra_names[3]}{}

template class Wrapper<AttributesWrapper>;
template class Wrapper<UniformsWrapper>;
