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
#include "costants.hpp"

class Chunk {
private:
    static constexpr int size = CHUNK_SIZE;

private:

    glm::ivec2 pq;
    static int min_y;
    static int max_y;

private:

    BlockMap map;
    BlockMap lights;

    int faces;
    int sign_faces;

    int dirty;

    const OpenglBuffer<CubeVertex> buffer;
    const Attrib attrib;
public:
    void draw();
    static int get_distance(const Chunk& c1, const Chunk& c2);
    static constexpr int getSize();
    static int getMinY();
    static int getMaxY();
};


#endif //CPP_CHUNK_HPP
