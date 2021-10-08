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

    using BlockMap = std::unordered_map<glm::ivec3, BlockType>;
    mutable BufferType local_buffer;
    BlockMap block_map{};
    mutable bool render_ready{false};
    mutable bool local_buffer_ready{false};

    static constexpr int Y_SIZE = 258;

    int count_exposed_faces() const;
    BufferType::iterator
    generate_block_geometry(BufferType::iterator vertex_it, const glm::ivec3 &block_pos, TileBlock tileBlock,
                            const std::array<bool, 6>& visible_faces) const;

    void compute_chunk_geometry() const;
    std::array<glm::ivec3, 8> get_chunk_boundaries() const;
    const std::array<glm::ivec2, 4> xz_boundaries;

    bool check_border(const glm::ivec3 &pos, const::glm::ivec3& direction) const;
    bool is_on_border(const glm::ivec3& pos) const;
    std::array<bool, 6> get_visible_faces(TileBlock w, const glm::ivec3 &pos) const;
public:
    Chunk(const glm::ivec2 &pq_coordinates);
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
    void update_buffer() const;
    bool is_dirty() const;

    static glm::ivec2 chunked(const glm::vec3& position);
    static int chunked(int val);

    void render_object() const;
    void init_chunk();
    static void wait_threads();
    void generate_blockmap();
};

int get_chunk_distance(const Chunk &c1, const Chunk &c2);

#endif //CPP_CHUNK_HPP
