//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>
#include <iostream>
#include <map>
#include "Geometry/Chunk.hpp"
#include "noise/noise.hpp"
#include "Geometry/CubicObject.hpp"

namespace CraftCpp {
Chunk::Chunk(const glm::ivec2 &pq_coordinates)
        : block_map{}, pq{pq_coordinates}, SuperClass{},
          xz_boundaries{
                  computeXZBoundaries(pq_coordinates)
          } {
};

int Chunk::getHighestBlock() const {
    int highest_y = -1;

    // choose the highest block in the block_map
    for (const auto &block: block_map) {
        const glm::ivec3 &block_pos{block.first};
        const TileBlock block_type{block.second};

        if (block_type.is_obstacle() && block_pos.y > highest_y)
            highest_y = block_pos.y;
    }
    return highest_y;
}

BlockType Chunk::get_block(const glm::ivec3 &block_pos) const {
    auto it = block_map.find(block_pos);
    return (it == block_map.end()) ? BlockType::EMPTY : it->second;
}

void Chunk::compute_chunk_geometry() const {
    int n_faces = count_exposed_faces();
    // each visible face has INDICES_FACE_COUNT indices that represent the triangle
    local_buffer = std::vector<CubeVertex>(n_faces * INDICES_FACE_COUNT);
    auto v_it = local_buffer.begin();

    for (const auto &kv: block_map) {
        const glm::ivec3 &block_pos{kv.first};
        const TileBlock &tileBlock{kv.second};

        // generate geometry of actual block (value returned by function is first free position in buffer)
        v_it = generate_block_geometry(v_it, block_pos, tileBlock, get_visible_faces(kv.second, kv.first));
    }
#ifndef NDEBUG
    std::cout << "chunk geometry computed\n";
#endif
    local_buffer_ready = true;
}

int Chunk::count_exposed_faces() const {
    int n_faces = 0;

    for (const auto &pair: block_map) {
        const auto &visible_faces = get_visible_faces(pair.second, pair.first);
        n_faces += std::accumulate(visible_faces.begin(), visible_faces.end(), 0);
    }
    return n_faces;
}

Chunk::BufferType::iterator
Chunk::generate_block_geometry(BufferType::iterator vertex_it, const glm::ivec3 &block_pos, TileBlock tileBlock,
                               const std::array<bool, 6> &visible_faces) const {
    if (tileBlock.is_plant()) {
        Plant plant{
                tileBlock.getBlockType(),
                block_pos,
                simplex2(block_pos.x, block_pos.z, 4, 0.5, 2) * 360,
                vertex_it,
                getLightObstacles(block_pos),
        };
        return plant.end();
    }
    if (tileBlock.is_empty() || visible_faces.empty()) {
        return vertex_it;
    }
    Cube cube{tileBlock.getBlockType(), visible_faces, block_pos, vertex_it, getLightObstacles(block_pos)};
    return cube.end();
}

void Chunk::update_buffer() const {
    SuperClass::update_buffer(local_buffer);
    render_ready = true;
#ifndef NDEBUG
    std::cout << "buffer updated\n";
#endif
}

void Chunk::generate_blockmap() {
    // for each block xz position
    for (int dx = 0; dx < Chunk::SIZE; dx++) {
        for (int dz = 0; dz < Chunk::SIZE; dz++) {
            int x = pq.x * Chunk::SIZE + dx;
            int z = pq.y * Chunk::SIZE + dz;

            float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
            float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
            int mh = g * 32 + 16;
            int h = f * mh;
            BlockType w = BlockType::GRASS;
            int t = 12;
            if (h <= t) {
                h = t;
                w = BlockType::SAND;
                set_block({x, h - 1, z}, static_cast<BlockType>(w));
            } else {
                for (int y = t - 1; y < h; y++) {
                    set_block({x, y, z}, static_cast<BlockType>(w));
                }
            }
            if (w == BlockType::GRASS) {
                if (SHOW_PLANTS) {
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        block_map[{x, h, z}] = static_cast<BlockType>(BlockType::TALL_GRASS);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        // w_f max == 23
                        int w_f = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        block_map[{x, h, z}] = static_cast<BlockType>(w_f);
                    }
                }
                bool ok = SHOW_TREES && !(dx - 4 < 0 || dz - 4 < 0 || dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE);
                ok = ok && (simplex2(x, z, 6, 0.5, 2) > 0.84);

                if (ok) {
                    for (int y = h + 3; y < h + 8; y++) {
                        for (int ox = -3; ox <= 3; ox++) {
                            for (int oz = -3; oz <= 3; oz++) {
                                int d = (ox * ox) + (oz * oz) +
                                        (y - (h + 4)) * (y - (h + 4));
                                if (d < 11) {
                                    block_map[{x + ox, y, z + oz}] = BlockType::LEAVES;
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        block_map[{x, y, z}] = BlockType::WOOD;
                    }
                }
            }
            if (SHOW_CLOUDS) {
                for (int y = 64; y < 72; y++) {
                    if (simplex3(
                            x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75) {
                        block_map[{x, y, z}] = BlockType::CLOUD;
                    }
                }
            }
        }
    }
#ifndef NDEBUG
    std::cout << "chunk initialized\n";
#endif
    compute_chunk_geometry();
}

bool Chunk::is_visible(const glm::mat4 &viewproj) const {
    for (const auto &p: xz_boundaries) {
        glm::vec4 clip_point = viewproj * glm::vec4{p[0], 0, p[1], 1};
        clip_point = glm::abs(clip_point) / clip_point.w;

        bool is_in = clip_point.x < 1 && clip_point.z < 1;
        if (is_in) {
            return true;
        }
    }
    return false;
}

std::array<glm::ivec3, 8> Chunk::get_chunk_boundaries() const {
    std::array<glm::ivec3, 8> boundaries{};
    auto it{boundaries.begin()};

    for (const auto &p: xz_boundaries) {
        *(it++) = glm::vec3{p[0], 0, p[1]};
    }
    for (const auto &p: xz_boundaries) {
        *(it++) = glm::vec3{p[0], getHighestBlock(), p[1]};
    }

    return boundaries;
}

void Chunk::set_block(const glm::ivec3 &position, BlockType w) {
    if (TileBlock(w).is_empty()) {
        block_map.erase(position);
        return;
    }
    block_map[position] = w;
}

int get_chunk_distance(const glm::ivec2 &pq1, const glm::ivec2 &pq2) {
    glm::ivec2 delta = pq1 - pq2;
    return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

int Chunk::chunked(int val) {
    return glm::floor(glm::round(val) / Chunk::SIZE);
}

glm::ivec2 Chunk::chunked(const glm::vec3 &position) {
    return {
            chunked(static_cast<int>(position.x)),
            chunked(static_cast<int>(position.z))
    };
}

void Chunk::render_object() const {
    if (local_buffer_ready) {
        if (!render_ready) {
            update_buffer();
        }
        SuperClass::render_object();
    }
}

void Chunk::init_chunk() {
    init_chunk_thread = std::thread(&Chunk::generate_blockmap, this);
}

glm::ivec2 Chunk::get_min_xz(const glm::ivec2 &pq) {
    return pq * SIZE - 1;
}

glm::ivec2 Chunk::get_max_xz(const glm::ivec2 &pq) {
    return get_min_xz(pq) + SIZE + 1;
}

int Chunk::get_min_x() const {
    return xz_boundaries[0][0];
}

int Chunk::get_min_z() const {
    return xz_boundaries[0][1];
}

int Chunk::get_max_x() const {
    return xz_boundaries[3][0];
}

int Chunk::get_max_z() const {
    return xz_boundaries[3][1];
}

bool Chunk::check_border(const glm::ivec3 &pos, const ::glm::ivec3 &direction) const {
    const glm::ivec3 new_pos = pos + direction;
    return
            new_pos.x <= get_min_x() || new_pos.x >= get_max_x() || new_pos.z <= get_min_z() ||
            new_pos.z >= get_max_z();
}

std::array<bool, 6> Chunk::get_visible_faces(TileBlock w, const glm::ivec3 &pos) const {
    static constexpr std::array<glm::ivec3, 6> offsets{{
                                                               {-1, 0, 0},
                                                               {1, 0, 0},
                                                               {0, 1, 0},
                                                               {0, -1, 0},
                                                               {0, 0, -1},
                                                               {0, 0, 1},
                                                       }};

    return {
            TileBlock{get_block(pos + offsets[0])}.is_transparent(),
            TileBlock{get_block(pos + offsets[1])}.is_transparent(),
            TileBlock{get_block(pos + offsets[2])}.is_transparent(),
            w == BlockType::CLOUD ||
            w == BlockType::LEAVES, //pos.y > 0 && TileBlock{get_block(pos + offsets[3])}.is_transparent(),
            TileBlock{get_block(pos + offsets[4])}.is_transparent(),
            TileBlock{get_block(pos + offsets[5])}.is_transparent(),
    };
}

bool Chunk::is_on_border(const glm::ivec3 &pos) const {
    return check_border(pos, {});
}

std::unordered_map<glm::ivec3, bool> Chunk::getLightObstacles(const glm::ivec3 &blockPos) const {
    std::unordered_map<glm::ivec3, bool> lightObstacles{};

    for (int ox = -1; ox <= 1; ++ox) {
        for (int oy = -1; oy <= 1; ++oy) {
            for (int oz = -1; oz <= 1; ++oz) {
                glm::ivec3 offset{ox, oy, oz};
                lightObstacles.emplace(offset, !TileBlock{get_block(blockPos + offset)}.is_transparent());
            }
        }
    }

    return lightObstacles;
}

bool Chunk::isLocalBufferReady() const {
    // check if thread has finished buffer modification
    return local_buffer_ready;
}

void Chunk::wait_thread() const {
    init_chunk_thread.join();
}

std::array<glm::ivec2, 4> Chunk::computeXZBoundaries(const glm::ivec2 &pq) {
    auto min{get_min_xz(pq)};
    auto max{get_max_xz(pq)};

    return {{
                    {min[0], min[1]},
                    {max[0], min[1]},
                    {min[0], max[1]},
                    {max[0], max[1]}
            }};
}

bool Chunk::needInitCall() const {
    return !(init_chunk_thread.joinable() || isLocalBufferReady());
}
}