//
// Created by ultimatenick on 15/08/21.
//

#include "AttributesWrapper.hpp"

AttributesWrapper::Iterator::Iterator(const value_type* actual_attribute) : actual_attribute{actual_attribute} {}

const AttributesWrapper::value_type &AttributesWrapper::Iterator::operator*() {
    return *actual_attribute;
}

const AttributesWrapper::value_type *AttributesWrapper::Iterator::operator->() {
    return actual_attribute;
}

AttributesWrapper::Iterator& AttributesWrapper::Iterator::operator++() {
    ++actual_attribute;
    return *this;
}

bool operator==(const AttributesWrapper::Iterator &a, const AttributesWrapper::Iterator &b) {
    return a.actual_attribute == b.actual_attribute;
}

bool operator!=(const AttributesWrapper::Iterator &a, const AttributesWrapper::Iterator &b) {
    return a.actual_attribute != b.actual_attribute;
}

AttributesWrapper::Iterator AttributesWrapper::begin() const{
    return {reinterpret_cast<const value_type*>(this)};
}

AttributesWrapper::Iterator AttributesWrapper::end() const{
    return {reinterpret_cast<const value_type*>(this) + sizeof(AttributesWrapper) / sizeof(value_type)};
}
