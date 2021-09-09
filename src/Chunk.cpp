//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>
#include <noise.h>

#include "Chunk.hpp"
#include "Model.hpp"
#include "CubicObject.hpp"

Chunk::Chunk(const Model &model, const glm::vec2 &pq, bool init) : model{model}, pq{pq},
                                                                   xz_boundaries{get_xz_boundaries(pq)}
{};

constexpr int Chunk::getSize() {
    return size;
}

int Chunk::getMinY() const{
    return min_y;
}

int Chunk::getMaxY() const{
    return max_y;
}

int Chunk::getHighestBlock() const{
    int highest_y = -1;

    for(const auto& rit : block_map){
        if((rit.second).is_obstacle() && rit.first.y > highest_y)
            highest_y = rit.first;
    }
    return highest_y;
}

TileBlock Chunk::get_block(const glm::ivec3& block_pos) const{
    return block_map.at(block_pos);
}

Chunk::operator bool() const {
    return !block_map.empty();
}

void Chunk::compute_chunk(const std::array<std::array<BlockMap*,3>,3>& neighbors_block_maps) {
    std::vector<bool> opaque(XZ_SIZE * XZ_SIZE * Y_SIZE);
    std::vector<char> highest(XZ_SIZE * XZ_SIZE);

    // position 0 - 1 for each coordinate
    glm::ivec3 offset{
        pq.x * Chunk::size - Chunk::size - 1,
        -1,
        pq.y * Chunk::size - Chunk::size - 1
    };

    populate_opaque(neighbors_block_maps, offset, opaque, highest);

    count_exposed_faces(block_map, opaque, offset);
    local_buffer = std::vector<CubeVertex>(faces * INDICES_FACE_COUNT);
    auto v_it = local_buffer.begin();

    for(const auto& kv : block_map){
        const glm::ivec3& e{kv.first};
        const TileBlock& tileBlock{kv.second};

        v_it = generate_geometry(opaque, v_it, e, highest, e - offset, tileBlock);
    }
}

void Chunk::populate_opaque(const std::array<std::array<BlockMap*,3>,3>& neighbors_block_maps, const glm::ivec3 &o, const std::vector<bool>& opaque, const std::vector<char>& highest) const {
    for(const auto& blockmaps_row : neighbors_block_maps){
        for(const auto bm_ref : blockmaps_row){
            if(!bm_ref){
                continue;
            }
            // kv is key-value (position-item)
            for(const auto& kv : *bm_ref){
                const glm::ivec3& e{kv.first};
                TileBlock tileBlock{kv.second};
                auto v = e - o;

                opaque[XYZ(v.x, v.y, v.z)] = !tileBlock.is_transparent();
                if (opaque[XYZ(v.x, v.y, v.z)]) {
                    // update highest
                    highest[XZ(v.x, v.z)] = glm::max(highest[XZ(v.x, v.z)], v.y);
                }
            }
        }
    }
}

void Chunk::count_exposed_faces(const BlockMap& map, std::vector<bool> opaque, const glm::ivec3& o) {
    int miny = 256; // max
    int maxy = 0;

    for(const auto& kv : map) {
        glm::ivec3 e{kv.first};
        TileBlock tileBlock{kv.second};
        auto v = e - o;

        // if block next to this face is opaque, no need to render
        std::array<bool, 6> f{
            !opaque[XYZ(v.x - 1, v.y, v.z)],
            !opaque[XYZ(v.x + 1, v.y, v.z)],
            !opaque[XYZ(v.x, v.y + 1, v.z)],
            !opaque[XYZ(v.x, v.y - 1, v.z)] && (e.y > 0), // no underground
            !opaque[XYZ(v.x, v.y, v.z - 1)],
            !opaque[XYZ(v.x, v.y, v.z + 1)]
        };

        int total = std::accumulate(f.begin(), f.end(), 0);
        if(total == 0){
            continue;
        }
        if(tileBlock.is_plant()){
            total = 4;
        }

        min_y = glm::min(min_y, e.y);
        max_y = glm::max(max_y, e.y);

        faces += total;
    }
}

decltype(Chunk::local_buffer)::iterator Chunk::generate_geometry(const std::vector<bool> &opaque, decltype(local_buffer)::iterator vertex_it, const glm::vec3& e,
                                                                 const std::vector<char> &highest, const glm::vec3& v, TileBlock w) {
    std::array<bool, 6> f{
            !opaque[XYZ(v.x - 1, v.y, v.z)],
            !opaque[XYZ(v.x + 1, v.y, v.z)],
            !opaque[XYZ(v.x, v.y + 1, v.z)],
            !opaque[XYZ(v.x, v.y - 1, v.z)] && (e.y > 0), // no underground
            !opaque[XYZ(v.x, v.y, v.z - 1)],
            !opaque[XYZ(v.x, v.y, v.z + 1)]
    };

    const decltype(local_buffer)::iterator end;
    if(w.is_plant()){
        auto plant = Plant(w.getIndex(), f, e, simplex2(e.x, e.z, 4, 0.5, 2) * 360, vertex_it);
        return plant.get_end();
    }
    else{
        auto cube = Cube(w.getIndex(), f, e, vertex_it);
        return cube.get_end();
    }
}

void Chunk::generate_chunk() {
    std::array<std::array<BlockMap*,3>,3> neighbor_block_maps{};

    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            const Chunk& other = *this;
            if(dq || dp){
                other = model.get_chunk_at(pq + glm::ivec2{dp, dq});
            }
            neighbor_block_maps[dp + 1][dq + 1] = (other) ? &(other.block_map) : nullptr;
        }
    }

    compute_chunk(neighbor_block_maps);
}

std::array<glm::vec3, 4> Chunk::get_xz_boundaries(const glm::vec2 &pq) {
    glm::vec3 min{pq[0] * Chunk::size - 1.0f, 0, pq[1] * Chunk::size - 1.0f};
    glm::vec3 max{min + static_cast<float>(Chunk::size + 1) * glm::vec3{1,0,1}};

    return {{
        {min.x, 0, min.z},
        {max.x, 0, min.z},
        {min.x, 0, max.z},
        {max.x, 0, max.z}
    }};
}

bool Chunk::is_visible(const Frustum& frustum) const {
    for(const auto& p : xz_boundaries){
        bool is_inside = frustum.is_inside(p + static_cast<float>(min_y));
        if(is_inside) {return true;}
    }
    for(const auto& p : xz_boundaries){
        bool is_inside = frustum.is_inside(p + static_cast<float>(max_y));
        if(is_inside) {return true;}
    }
    return false;
}

void Chunk::update_buffer() {
    generate_chunk();
    SuperClass::update_buffer();
    dirty = false;
}

bool Chunk::is_dirty() const {
    return dirty;
}

void Chunk::init() {
    const int pad = 1;
    for(int dx{-pad}; dx < Chunk::size ; dx++){
        for(int dz{-pad}; dz < Chunk::size ; dz++){
            bool on_edge_flag = (dx < 0 || dz < 0 || dx >= Chunk::size || dz >= Chunk::size);
            int x = pq.x * Chunk::size + dx;
            int z = pq.y * Chunk::size + dz;

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
            // sand and grass terrain
            for(int y = 0 ; y < h ; y++){
                block_map.set_tileBlock({x,y,z}, static_cast<int>(w) * on_edge_flag);
            }
            if(w == BlockType::GRASS){
                if(SHOW_PLANTS){
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        block_map.set_tileBlock({x, h, z}, static_cast<int>(BlockType::TALL_GRASS) * on_edge_flag);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        // w_f max == 23
                        int w_f = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        block_map.set_tileBlock({x, h, z}, w_f * on_edge_flag);
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
                                    block_map.set_tileBlock({x + ox, y, z + oz}, BlockType::LEAVES);
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        block_map.set_tileBlock({x, y, z}, BlockType::WOOD);
                    }
                }
                if (SHOW_CLOUDS) {
                    for (int y = 64; y < 72; y++) {
                        if (simplex3(
                                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                        {
                            block_map.set_tileBlock({x, y, z}, 16);
                        }
                    }
                }
            }
        }
    }
}

void Chunk::set_block(const glm::ivec3& position, const TileBlock& w) {
    block_map.set_tileBlock(position, w);
    dirty = true;
}
