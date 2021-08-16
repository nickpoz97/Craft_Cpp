//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <vec2.hpp>

#include "BlockMap.hpp"
#include "list"
#include "Sign.hpp"
#include "Attrib.hpp"
#include "OpenglBuffer.hpp"
#include "Vertex.hpp"

class Chunk {
private:
    static constexpr int CHUNK_SIZE = 32;

    glm::ivec2 id;          //pq
    glm::ivec2 min;

    BlockMap map;
    BlockMap lights;

    int faces;
    int sign_faces;

    int dirty;

    const OpenglBuffer<CubeVertex> buffer;
    const Attrib attrib;
public:
    void draw();
};


#endif //CPP_CHUNK_HPP
