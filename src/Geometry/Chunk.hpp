//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <array>
#include <thread>
#include <unordered_map>
#include "vec2.hpp"
#include "map_utils.hpp"
#include "list"
#include "RenderableEntity.hpp"
#include "Vertex.hpp"
#include "../costants.hpp"

class Chunk;
using ChunkMap = std::unordered_map<glm::ivec2, Chunk>;

class Chunk : public RenderableEntity<CubeVertex>{
private:
    static inline std::list<std::thread> init_chunk_threads{};
    using SuperClass = RenderableEntity<CubeVertex>;
    using BufferType = std::vector<CubeVertex>;

    using MyMap = std::unordered_map<glm::ivec3, BlockType>;
    MyMap block_map{};
    mutable bool dirty{false};

    // XZ_SIZE is the size of 3 chunks
    static constexpr int XZ_SIZE = CHUNK_SIZE * 3 + 2;
    static constexpr int Y_SIZE = 258;
    //static constexpr auto XYZ = [](int x, int y, int z){ return y * XZ_SIZE * XZ_SIZE + x * XZ_SIZE + z; };
    //static constexpr auto XZ = [](int x, int z){ return XYZ(x,0,z); };

    using opaque_matrix_type = std::array<std::array<std::array<bool,XZ_SIZE>,Y_SIZE>,XZ_SIZE>;
    using height_matrix_type = std::array<std::array<char,XZ_SIZE>,XZ_SIZE>;

    void populate_opaque_and_height_matrix(
        const std::array<const Chunk*, 6> &np,
        const glm::ivec3 &offset,
        opaque_matrix_type &opaque,
        height_matrix_type &highest
    ) const;
    int count_exposed_faces(const MyMap& map, const opaque_matrix_type &opaque, const glm::ivec3& offset) const;
    BufferType::iterator generate_block_geometry(const opaque_matrix_type &opaque, BufferType::iterator vertex_it, const glm::ivec3& block_abs_pos,
                                                             const height_matrix_type &highest, const glm::ivec3& v, TileBlock w) const;

    BufferType compute_chunk_geometry(const std::array<const Chunk*, 6> &np) const;
    std::array<glm::ivec3, 8> get_chunk_boundaries() const;
    const std::array<glm::ivec2, 4> xz_boundaries;

    bool check_border(const glm::ivec3 &pos, const::glm::ivec3& direction) const;
    bool is_on_border(const glm::ivec3& pos) const;
    std::array<bool, 6> get_visible_faces(TileBlock w, const glm::ivec3 &pos, const ChunkMap &chunkMap) const;
    BlockType get_block(const glm::ivec3 &pos, const ChunkMap& chunk_map) const;
public:
    Chunk(const glm::ivec2 &pq_coordinates, bool init);
    const glm::ivec2 pq;

    int get_min_x() const;
    int get_min_z() const;
    int get_max_x() const;
    int get_max_z() const;

    static constexpr int SIZE = CHUNK_SIZE;
    static glm::ivec2 get_min_xz(const glm::ivec2& pq);
    static glm::ivec2 get_max_xz(const glm::ivec2& pq);

    static constexpr auto get_y_limit = [](){return Y_SIZE - 2;};
    int getHighestBlock() const;
    BlockType get_block(const glm::ivec3& block_pos) const;

    operator bool() const;

    void set_block(const glm::ivec3& position, BlockType w);
    //bool is_visible(const Frustum& frustum) const;
    bool is_visible(const glm::mat4 &viewproj) const;
    void update_buffer(const std::array<const Chunk*, 6> &np) const;
    bool is_dirty() const;

    static glm::ivec2 chunked(const glm::vec3& position);
    static int chunked(int val);

    void render_object(const std::array<const Chunk*, 6>& neighbors_refs) const;
    void init_chunk();
    static void wait_threads();
    void generate_blockmap();
};

int get_chunk_distance(const Chunk &c1, const Chunk &c2);

#endif //CPP_CHUNK_HPP
