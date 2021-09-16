//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <array>

#include "vec2.hpp"
#include "BlockMap.hpp"
#include "list"
#include "RenderableEntity.hpp"
#include "Vertex.hpp"
#include "costants.hpp"

class Chunk : public RenderableEntity<CubeVertex>{
private:
    using SuperClass = RenderableEntity<CubeVertex>;
    using neighbors_pointers = std::array<std::array<const BlockMap*,3>,3>;

    std::vector<CubeVertex> local_buffer;

    int min_y{};
    int max_y{};
    std::array<glm::vec3, 4> xz_boundaries;

    BlockMap block_map;
    bool dirty{false};

    int n_faces{}; // n of faces

    // XZ_SIZE is the size of 3 chunks
    static constexpr int XZ_SIZE = CHUNK_SIZE * 3 + 2;
    static constexpr int Y_SIZE = 258;
    //static constexpr auto XYZ = [](int x, int y, int z){ return y * XZ_SIZE * XZ_SIZE + x * XZ_SIZE + z; };
    //static constexpr auto XZ = [](int x, int z){ return XYZ(x,0,z); };
    static std::array<glm::vec3, 4> get_xz_boundaries(const glm::vec2 &pq);

    using opaque_matrix_type = std::array<std::array<std::array<bool,XZ_SIZE>,Y_SIZE>,XZ_SIZE>;
    using height_matrix_type = std::array<std::array<char,XZ_SIZE>,XZ_SIZE>;

    static void populate_opaque_and_height_matrix(
            const std::array<std::array<const BlockMap *, 3>, 3> &neighbors_block_maps, const glm::ivec3 &offset,
            opaque_matrix_type &opaque, height_matrix_type &highest) ;
    void count_exposed_faces(const BlockMap& map, const opaque_matrix_type &opaque, const glm::ivec3& offset);
    decltype(local_buffer)::iterator generate_block_geometry(const opaque_matrix_type &opaque, decltype(local_buffer)::iterator vertex_it, const glm::ivec3& block_abs_pos,
                                                             const height_matrix_type &highest, const glm::ivec3& v, TileBlock w);

    std::array<glm::vec3, 8> get_chunk_boundaries() const;
public:
    Chunk(const glm::vec2 &pq_coordinates, bool init);
    const glm::ivec2 pq;
    static constexpr int SIZE = CHUNK_SIZE;

    int get_min_y() const;
    int get_max_y() const;
    int getHighestBlock() const;
    TileBlock get_block(const glm::ivec3& block_pos) const;
    explicit operator bool() const;

    void compute_chunk_geometry(const neighbors_pointers &neighbors_block_maps);

    void set_block(const glm::ivec3& position, BlockType w);
    //bool is_visible(const Frustum& frustum) const;
    bool is_visible(const glm::mat4 &viewproj) const;
    void update_buffer(const neighbors_pointers &np);
    bool is_dirty() const;

    void init_chunk();
    friend int get_chunk_distance(const Chunk& c1, const Chunk& c2);

    static glm::ivec2 chunked(const glm::vec3& position);
    static int chunked(int val);
};

int get_chunk_distance(const Chunk& c1, const Chunk& c2);

#endif //CPP_CHUNK_HPP
