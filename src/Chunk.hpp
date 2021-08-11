//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <vec2.hpp>

#include "BlockMap.hpp"
#include "list"
#include "Sign.hpp"

class Chunk {
private:
    static constexpr int CHUNK_SIZE = 32;

    glm::ivec2 coordinates;
    glm::ivec2 min;

    BlockMap map;
    BlockMap lights;

    int faces;
    int sign_faces;

    int dirty;
};


#endif //CPP_CHUNK_HPP
