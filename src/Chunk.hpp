//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include "vec2.hpp"

#include "BlockMap.hpp"
#include "list"
#include "Sign.hpp"
#include "AttributesWrapper.hpp"
#include "OpenglBuffer.hpp"
#include "Vertex.hpp"
#include "costants.hpp"
#include "Model.hpp"

class Chunk {
private:
    //const WorkerItem& wi;

    //OpenglBuffer<Uv3DVertex> sign_buffer;
    OpenglBuffer<CubeVertex> gpu_buffer{};
    std::vector<CubeVertex> local_buffer{};

    const Model& model;
    const glm::ivec2 pq;

    int min_y{};
    int max_y{};

    BlockMap blockMap{};
    //BlockMap lights{};
    //std::list<Sign> sign_list{};

    int faces{}; // n of faces
    //int sign_faces;

    //bool dirty;
    //const Attrib attrib;

    static constexpr int XZ_SIZE = Chunk::size * 3 + 2;
    static constexpr int XZ_LO = Chunk::size;
    static constexpr int XZ_HI = Chunk::size * 2 + 1;
    static constexpr int Y_SIZE = 258;
    static constexpr auto XYZ = [](int x, int y, int z){ return y * XZ_SIZE * XZ_SIZE + x * XZ_SIZE + z; };
    static constexpr auto XZ = [](int x, int z){ return XYZ(x,0,z); };

    void populate_opaque(const WorkerItem &wi, const glm::ivec3 &o, const std::vector<bool>& opaque, const std::vector<char>& highest) const;
    void count_exposed_faces(const BlockMap& map, std::vector<bool> opaque, const glm::ivec3& o);
    decltype(local_buffer)::iterator generate_geometry(const std::vector<bool> &opaque, decltype(local_buffer)::iterator vertex_it, const glm::vec3& e,
                                                       const std::vector<char> &highest, const glm::vec3& v, TileBlock w);
    //static void void light_fill(std::vector<char>& opaque, std::vector<char>& light, const glm::vec3& v, int w, bool force);
public:
    static constexpr int size = CHUNK_SIZE;

    Chunk(const Model& model, const glm::vec2 &pq);
    void render(const glm::mat4& transform);
    static constexpr int getSize();
    int getMinY() const;
    int getMaxY() const;
    Tile getHighestBlock() const;
    const glm::ivec2 &getPq() const;
    TileBlock get_block(const glm::ivec3& block_pos) const;
    operator bool() const;
    //void gen_sign_buffer();
    //bool has_lights();
    //void set_dirt();

    void compute_chunk(const WorkerItem &wi);
    void generate_chunk();
    void generate_buffer();

    void set_block(const glm:ivec3& chunked_position, const TileBlock& w);
};


#endif //CPP_CHUNK_HPP
