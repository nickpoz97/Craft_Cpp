//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP


#include "BlockMap.hpp"
#include "list"
#include "Sign.hpp"

class Chunk {
private:
    static constexpr int CHUNK_SIZE = 32;

    Vec<int,2> coordinates;
    Vec<int,2> min;

    BlockMap map;
    BlockMap lights;

    int faces;
    int sign_faces;

    int dirty;
};


#endif //CPP_CHUNK_HPP
