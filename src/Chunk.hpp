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
#include "Model.hpp"

class Chunk {
private:
    OpenglBuffer<Uv3DVertex> sign_buffer;
    static constexpr int size = CHUNK_SIZE;
    const Model& model;

    const glm::ivec2 pq;
    static int min_y;
    static int max_y;

    BlockMap map;
    BlockMap lights;
    std::list<Sign> sign_list;

    int faces;
    int sign_faces;

    bool dirty;

    const OpenglBuffer<CubeVertex> buffer;
    const Attrib attrib;
public:
    void draw();
    static constexpr int getSize();
    static int getMinY();
    static int getMaxY();
    Item getHighestBlock() const;
    const glm::ivec2 &getPq() const;
    Item get_block(const glm::ivec3& block_pos) const;
    bool operator!() const;
    void gen_sign_buffer();
    bool has_lights();
    void set_dirt();
};


#endif //CPP_CHUNK_HPP
