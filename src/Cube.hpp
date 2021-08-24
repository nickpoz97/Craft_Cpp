//
// Created by ultimatenick on 29/07/21.
//

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>
#include <vec3.hpp>
#include <vec2.hpp>

#include "item.hpp"
#include "CubicObject.hpp"
#include "Block.hpp"

class Cube : public CubicObject<6, INDICES_FACE_COUNT>{
private:
    using SuperClass = CubicObject<6, INDICES_FACE_COUNT>;

    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic

    IndicesMatrix true_indices(const LightMatrix&  ao);
public:
    Cube(const TileBlock& tiles, const LightMatrix&  ao, const LightMatrix&  light);
};


#endif //CPP_CUBE_HPP
