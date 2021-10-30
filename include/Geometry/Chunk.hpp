//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <array>
#include <thread>
#include <unordered_map>
#include <list>

#include "glm/vec2.hpp"
#include "Geometry/map_utils.hpp"
#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "tunable_parameters.hpp"

namespace CraftCpp {
class Chunk;

using ChunkMap = std::unordered_map<glm::ivec2, Chunk>;

class Chunk : public RenderableEntity<CubeVertex> {
private:
    mutable std::thread init_chunk_thread{};
    using SuperClass = RenderableEntity<CubeVertex>;
    using BufferType = std::vector<CubeVertex>;

    using BlockMap = std::unordered_map<glm::ivec3, BlockType>;
    mutable BufferType local_buffer;
    BlockMap block_map{};

    mutable bool render_ready{false};
    mutable bool local_buffer_ready{false};

    int count_exposed_faces() const;

    BufferType::iterator
    generate_block_geometry(BufferType::iterator vertex_it, const glm::ivec3 &block_pos, TileBlock tileBlock,
                            const std::array<bool, 6> &visible_faces) const;

    void compute_chunk_geometry() const;

    std::array<glm::ivec3, 8> get_chunk_boundaries() const;

    const std::array<glm::ivec2, 4> xz_boundaries;

    bool check_border(const glm::ivec3 &pos, const ::glm::ivec3 &direction) const;

    std::array<bool, 6> get_visible_faces(TileBlock w, const glm::ivec3 &pos) const;

public:
    explicit Chunk(const glm::ivec2 &pq_coordinates);
    static constexpr int SIZE = CHUNK_SIZE;
    static constexpr int Y_LIMIT = 256;

    const glm::ivec2 pq;

    int get_min_x() const;

    int get_min_z() const;

    int get_max_x() const;

    int get_max_z() const;

    static std::array<glm::ivec2, 4> computeXZBoundaries(const glm::ivec2 &pq);

    static glm::ivec2 get_min_xz(const glm::ivec2 &pq);

    static glm::ivec2 get_max_xz(const glm::ivec2 &pq);

    int getHighestBlock() const;

    BlockType get_block(const glm::ivec3 &block_pos) const;

    void set_block(const glm::ivec3 &position, BlockType w);

    //bool is_visible(const Frustum& frustum) const;
    bool is_visible(const glm::mat4 &viewproj) const;

    void update_buffer() const;

    bool isLocalBufferReady() const;

    // thread not running and not initialized
    bool needInitCall() const;

    static glm::ivec2 chunked(const glm::vec3 &position);

    static int chunked(int val);

    void render_object() const;

    void init_chunk();

    void generate_blockmap();

    // first is top-left, going counterclockwise
    std::unordered_map<glm::ivec3, bool> getLightObstacles(const glm::ivec3 &blockPos) const;

    void wait_thread() const;

    bool is_on_border(const glm::ivec3 &pos) const;
};

int get_chunk_distance(const glm::ivec2 &pq1, const glm::ivec2 &pq2);

}

#endif //CPP_CHUNK_HPP
