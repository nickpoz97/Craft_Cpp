//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>

#include "noise.h"
#include "Chunk.hpp"
#include "Model.hpp"
#include "CubicObject.hpp"

Chunk::Chunk(const glm::vec2 &pq_coordinates, bool init) : block_map{pq_coordinates}, pq{pq_coordinates},
                                                           xz_boundaries{get_xz_boundaries(pq_coordinates)},
                                                           SuperClass{local_buffer}
{
    if(init){init_chunk();}
};

int Chunk::get_min_y() const{
    return min_y;
}

int Chunk::get_max_y() const{
    return max_y;
}

int Chunk::getHighestBlock() const{
    int highest_y = -1;

    // choose the highest block in the block_map
    for(const auto& block : block_map){
        const glm::vec3& block_pos{block.first};
        const TileBlock block_type{block.second};

        if(block_type.is_obstacle() && block_pos.y > highest_y)
            highest_y = block_pos.y;
    }
    return highest_y;
}

TileBlock Chunk::get_block(const glm::ivec3& block_pos) const{
    return block_map.at(block_pos);
}

Chunk::operator bool() const {
    // check if Chunk contains at least 1 block
    return !block_map.empty();
}

// TODO do not use model
void Chunk::compute_chunk_geometry(const Model &model) {
    std::array<std::array<const BlockMap*,3>,3> neighbor_block_maps{};

    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            const Chunk& other = *this;
            if(dq || dp){
                other = model.get_chunk_at(pq + glm::ivec2{dp, dq});
            }
            neighbor_block_maps[dp + 1][dq + 1] = (other) ? &(other.block_map) : nullptr;
        }
    }

    _compute_chunk_geometry(neighbor_block_maps);
}

void Chunk::_compute_chunk_geometry(const std::array<std::array<const BlockMap *, 3>, 3> &neighbors_block_maps) {
    opaque_matrix_type opaque{};
    height_matrix_type highest{};

    // offset is the beginning of the chunk
    glm::ivec3 offset{
        pq.x * Chunk::SIZE - Chunk::SIZE - 1,
        -1,
        pq.y * Chunk::SIZE - Chunk::SIZE - 1
    };

    // this matrices store info about visibility
    populate_opaque_and_height_matrix(neighbors_block_maps, offset, opaque, highest);

    count_exposed_faces(block_map, opaque, offset);
    // each visible face has INDICES_FACE_COUNT indices that represent the triangle
    local_buffer = std::vector<CubeVertex>(n_faces * INDICES_FACE_COUNT);
    auto v_it = local_buffer.begin();

    for(const auto& kv : block_map){
        const glm::ivec3& block_pos{kv.first};
        const TileBlock& tileBlock{kv.second};

        // generate geometry of actual block (value returned by function is first free position in buffer)
        v_it = generate_block_geometry(opaque, v_it, block_pos, highest, block_pos - offset, tileBlock);
    }
}

void Chunk::populate_opaque_and_height_matrix(const std::array<std::array<const BlockMap *, 3>, 3> &neighbors_block_maps,
                                              const glm::ivec3 &offset,
                                              opaque_matrix_type &opaque,
                                              height_matrix_type &highest) {
    // analyze neighbors maps
    for(const auto& blockmaps_row : neighbors_block_maps){
        for(const auto bm_ref : blockmaps_row){
            // check if neighbor is not instantiated
            if(!bm_ref){
                continue;
            }
            // kv is key-value (position-item)
            for(const auto& kv : *bm_ref){
                const glm::ivec3& block_pos{kv.first};
                const TileBlock& tileBlock{kv.second};
                // v is the pos relative to the beginning of the chunk (o)
                auto v = block_pos - offset;

                opaque[v.x][v.y][v.z] = !tileBlock.is_transparent();
                if (opaque[v.x][v.y][v.z]) {
                    // update highest block in the xz position
                    highest[v.x][v.z] = glm::max(highest[v.x][v.z], static_cast<char>(v.y));
                }
            }
        }
    }
}

void Chunk::count_exposed_faces(const BlockMap& map, const opaque_matrix_type &opaque, const glm::ivec3& offset) {
    int miny = 256;
    int maxy = 0;

    for(const auto& kv : map) {
        glm::ivec3 abs_pos{kv.first};
        TileBlock tileBlock{kv.second};
        auto v = abs_pos - offset;

        // if block next to this face is opaque, no need to render
        std::array<bool, 6> f{
            !opaque[v.x - 1][v.y][v.z],
            !opaque[v.x + 1][v.y][v.z],
            !opaque[v.x][v.y + 1][v.z],
            !opaque[v.x][v.y - 1][v.z] && (abs_pos.y > 0), // no underground
            !opaque[v.x][v.y][v.z - 1],
            !opaque[v.x][v.y][v.z + 1]
        };

        // count if almost one side is visible
        int total = std::accumulate(f.begin(), f.end(), 0);
        if(total == 0){
            continue;
        }
        if(tileBlock.is_plant()){
            total = 4;
        }

        min_y = glm::min(min_y, abs_pos.y);
        max_y = glm::max(max_y, abs_pos.y);

        n_faces += total;
    }
}

decltype(Chunk::local_buffer)::iterator Chunk::generate_block_geometry(const opaque_matrix_type &opaque,
                                                                       decltype(local_buffer)::iterator vertex_it,
                                                                       const glm::ivec3& block_abs_pos,
                                                                       const height_matrix_type &highest,
                                                                       const glm::ivec3& v,
                                                                       TileBlock w) {
    std::array<bool, 6> f{
            !opaque[v.x - 1][v.y][v.z],
            !opaque[v.x + 1][v.y][v.z],
            !opaque[v.x][v.y + 1][v.z],
            !opaque[v.x][v.y - 1][v.z] && (block_abs_pos.y > 0), // no underground
            !opaque[v.x][v.y][v.z - 1],
            !opaque[v.x][v.y][v.z + 1]
    };

    if(w.is_plant()){
        auto plant = Plant(w.getIndex(), f, block_abs_pos, simplex2(block_abs_pos.x, block_abs_pos.z, 4, 0.5, 2) * 360, vertex_it);
        return plant.get_end();
    }
    else{
        auto cube = Cube(w.getIndex(), f, block_abs_pos, vertex_it);
        return cube.get_end();
    }
}

std::array<glm::vec3, 4> Chunk::get_xz_boundaries(const glm::vec2 &pq) {
    glm::vec3 min{pq[0] * Chunk::SIZE - 1.0f, 0, pq[1] * Chunk::SIZE - 1.0f};
    glm::vec3 max{min + static_cast<float>(Chunk::SIZE + 1) * glm::vec3{1, 0, 1}};

    return {{
        {min.x, 0, min.z},
        {max.x, 0, min.z},
        {min.x, 0, max.z},
        {max.x, 0, max.z}
    }};
}

/*bool Chunk::is_visible(const Frustum& frustum) const {
    for(const auto& p : xz_boundaries){
        bool is_inside = frustum.is_inside(p + static_cast<float>(min_y));
        if(is_inside) {return true;}
    }
    for(const auto& p : xz_boundaries){
        bool is_inside = frustum.is_inside(p + static_cast<float>(max_y));
        if(is_inside) {return true;}
    }
    return false;
}*/

void Chunk::update_buffer(const Model &model) {
    compute_chunk_geometry(model);
    SuperClass::update_buffer(local_buffer);
    dirty = false;
}

bool Chunk::is_dirty() const {
    return dirty;
}

void Chunk::init_chunk() {
    const int pad = 1;
    // for each block xz position
    for(int dx{-pad}; dx < Chunk::SIZE + pad ; dx++){
        for(int dz{-pad}; dz < Chunk::SIZE + pad ; dz++){
            bool on_edge_flag = (dx < 0 || dz < 0 || dx >= Chunk::SIZE || dz >= Chunk::SIZE);
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
            }
            for(int y = 0 ; y < h ; y++){
                // TODO check if borders are empty
                block_map.set_block({x, y, z}, static_cast<BlockType>(w * on_edge_flag));
            }
            if(w == BlockType::GRASS){
                if(SHOW_PLANTS){
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        block_map.set_block({x, h, z},static_cast<BlockType>(BlockType::TALL_GRASS * on_edge_flag));
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        // w_f max == 23
                        int w_f = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        block_map.set_block({x, h, z}, static_cast<BlockType>(w_f * on_edge_flag));
                    }
                }
                bool ok = SHOW_TREES && !(dx - 4 < 0 || dz - 4 < 0 || dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE);
                ok = ok && (simplex2(x, z, 6, 0.5, 2) > 0.84);

                if(ok){
                    for(int y = h + 3; y < h + 8; y++){
                        for (int ox = -3; ox <= 3; ox++) {
                            for (int oz = -3; oz <= 3; oz++) {
                                int d = (ox * ox) + (oz * oz) +
                                        (y - (h + 4)) * (y - (h + 4));
                                if (d < 11) {
                                    block_map.set_block({x + ox, y, z + oz}, BlockType::LEAVES);
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        block_map.set_block({x, y, z},BlockType::WOOD);
                    }
                }
                if (SHOW_CLOUDS) {
                    for (int y = 64; y < 72; y++) {
                        if (simplex3(
                                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                        {
                            block_map.set_block({x, y, z}, BlockType::CLOUD);
                        }
                    }
                }
            }
        }
    }
}

bool Chunk::is_visible(const glm::mat4 &viewproj) const {
    for(const auto& p : get_chunk_boundaries()){
        glm::vec4 clip_point = viewproj * glm::vec4{(p + static_cast<float>(min_y)), 1};
        float clip_w = clip_point.w;
        clip_point = glm::abs(clip_point);

        if(clip_point.x < clip_w && clip_point.y < clip_w && clip_point.z < clip_w){
            return true;
        }
    }
    return false;
}

std::array<glm::vec3, 8> Chunk::get_chunk_boundaries() const {
    std::array<glm::vec3, 8> boundaries{};
    auto it{boundaries.begin()};

    for(const auto& p : xz_boundaries){
        *(it++) = p + static_cast<float>(min_y);
    }
    for(const auto& p : xz_boundaries){
        *(it++) = p + static_cast<float>(max_y);
    }

    return boundaries;
}

void Chunk::set_block(const glm::ivec3& position, BlockType w) {
    block_map.set_block(position, w);
    dirty = true;
}

int get_chunk_distance(const Chunk& c1, const Chunk& c2) {
    auto delta = c1.pq - c2.pq;
    return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

int Chunk::chunked(int val) {
    return glm::floor(glm::round(val) / Chunk::SIZE);
}

glm::ivec2 Chunk::chunked(const glm::vec3& position) {
    return {
            chunked(static_cast<int>(position.x)),
            chunked(static_cast<int>(position.z))
    };
}


