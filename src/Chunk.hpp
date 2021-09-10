//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include "vec2.hpp"

#include "BlockMap.hpp"
#include "list"
#include "OpenglBuffer.hpp"
#include "Vertex.hpp"
#include "costants.hpp"
#include "Model.hpp"

class Chunk : public GameObject<CubeVertex>{
private:
    using SuperClass = GameObject<CubeVertex>;

    std::vector<CubeVertex> local_buffer{};

    const Model& model;

    int min_y{};
    int max_y{};
    std::array<glm::vec3, 4> xz_boundaries;

    BlockMap block_map{};
    bool dirty{false};

    int faces{}; // n of faces

    static constexpr int XZ_SIZE = Chunk::size * 3 + 2;
    static constexpr int XZ_LO = Chunk::size;
    static constexpr int XZ_HI = Chunk::size * 2 + 1;
    static constexpr int Y_SIZE = 258;
    static constexpr auto XYZ = [](int x, int y, int z){ return y * XZ_SIZE * XZ_SIZE + x * XZ_SIZE + z; };
    static constexpr auto XZ = [](int x, int z){ return XYZ(x,0,z); };
    static std::array<glm::vec3, 4> get_xz_boundaries(const glm::vec2 &pq);

    void populate_opaque(const std::array<std::array<BlockMap*,3>,3>& neighbors_block_maps, const glm::ivec3 &o, const std::vector<bool>& opaque, const std::vector<char>& highest) const;
    void count_exposed_faces(const BlockMap& map, std::vector<bool> opaque, const glm::ivec3& o);
    decltype(local_buffer)::iterator generate_geometry(const std::vector<bool> &opaque, decltype(local_buffer)::iterator vertex_it, const glm::vec3& e,
                                                       const std::vector<char> &highest, const glm::vec3& v, TileBlock w);

    std::array<glm::vec3, 8> get_chunk_boundaries() const;
public:

    Chunk(const Model &model, const glm::vec2 &pq, bool init);
    const glm::ivec2 pq;

    static constexpr int size = CHUNK_SIZE;
    static constexpr int getSize();
    int getMinY() const;
    int getMaxY() const;
    int getHighestBlock() const;
    TileBlock get_block(const glm::ivec3& block_pos) const;
    operator bool() const;

    void compute_chunk(const std::array<std::array<BlockMap*,3>,3>&);
    void generate_chunk();

    void set_block(const glm:ivec3& position, const TileBlock& w);
    bool is_visible(const Frustum& frustum) const;
    bool is_visible(const glm::mat4 &viewproj) const;
    void update_buffer();
    bool is_dirty() const;

    void init();
};


#endif //CPP_CHUNK_HPP
