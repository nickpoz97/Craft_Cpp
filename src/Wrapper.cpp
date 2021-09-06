//
// Created by ultimatenick on 01/09/21.
//

#include "Wrapper<WrapperType>.hpp"
#include "Wrapper.hpp"


template<typename WrapperType>
Wrapper<WrapperType>::Iterator::Iterator(const value_type* actual_attribute) : actual_attribute{actual_attribute} {}

template<typename WrapperType>
Wrapper<WrapperType>::value_type &Wrapper<WrapperType>::Iterator::operator*() {
    return *actual_attribute;
}

template<typename WrapperType>
Wrapper<WrapperType>::value_type *Wrapper<WrapperType>::Iterator::operator->() {
    return actual_attribute;
}

template<typename WrapperType>
Wrapper<WrapperType>::Iterator& Wrapper<WrapperType>::Iterator::operator++() {
    ++actual_attribute;
    return *this;
}

template<typename WrapperType>
bool operator==(const Wrapper::Iterator &a, const Wrapper<WrapperType>::Iterator &b) {
    return a.actual_attribute == b.actual_attribute;
}

template<typename WrapperType>
bool operator!=(const Wrapper<WrapperType>::Iterator &a, const Wrapper<WrapperType>::Iterator &b) {
    return a.actual_attribute != b.actual_attribute;
}

template<typename WrapperType>
size_t Wrapper<WrapperType>::get_size() const{
    return sizeof(WrapperType);
}

template<typename WrapperType>
Wrapper<WrapperType>::Iterator Wrapper<WrapperType>::begin() const{
    return {reinterpret_cast<const value_type*>(this)};
}

template<typename WrapperType>
Wrapper<WrapperType>::Iterator Wrapper<WrapperType>::end() const{
    return {reinterpret_cast<const value_type*>(this) + (get_size() / sizeof(value_type))};
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
