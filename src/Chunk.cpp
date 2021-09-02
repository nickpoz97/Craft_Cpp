//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>
#include <noise.h>

#include "Chunk.hpp"
#include "Model.hpp"
#include "CubicObject.hpp"

Chunk::Chunk(const Model &model, const glm::vec2 &pq) : model{model},  pq{pq},
    xz_boundaries{get_xz_boundaries()}
{};

void Chunk::render() {
    gpu_buffer.draw_triangles();
}

constexpr int Chunk::getSize() {
    return size;
}

int Chunk::getMinY() const{
    return min_y;
}

int Chunk::getMaxY() const{
    return max_y;
}

Tile Chunk::getHighestBlock() const{
    for(auto rit{blockMap.rbegin()} ; rit != blockMap.rend() ; ++rit){
        if((rit->second).is_obstacle())
            return rit->second;
    }
    return Tile{};
}

const glm::ivec2 &Chunk::getPq() const {
    return pq;
}

TileBlock Chunk::get_block(const glm::ivec3& block_pos) const{
    return blockMap.at(block_pos);
}

Chunk::operator bool() const {
    return !blockMap.empty();
}

// checks if one of the neighbors has lights
/*bool Chunk::has_lights() {
    if(!model.show_light){
        return false;
    }
    const auto& chunks_map = model.getChunks();

    // iterate through neighbors
    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            if (dp || dq) {
                auto search_result = chunks_map.find(pq.x + dp, pq.y + dq);
                if(search_result == chunks_map.end()){
                    continue;
                }
                Chunk& other = *search_result;
                if(!other.lights.empty()){
                    return true;
                }
            }
        }
    }
    return false;
}*/

/*void Chunk::set_dirt() {
    dirty = true;
    if(!has_lights()){
        return;
    }
    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            auto search_result{model.getChunks().find(pq.x + dp, pq.y + dq)};
            if(search_result != model.getChunks().end()){
                search_result->dirty = true;
            }
        }
    }
}*/

void Chunk::compute_chunk(const WorkerItem &wi) {
    std::vector<bool> opaque(XZ_SIZE * XZ_SIZE * Y_SIZE);
    std::vector<char> highest(XZ_SIZE * XZ_SIZE);

    // position 0 - 1 for each coordinate
    glm::ivec3 offset{
        wi.pq_coordinates.x * Chunk::size - Chunk::size - 1,
        -1,
        wi.pq_coordinates.y * Chunk::size - Chunk::size - 1
    };

    bool has_light = wi.has_light();

    populate_opaque(wi, offset, opaque, highest);

    auto map = *(wi.block_maps[1][1]);
    count_exposed_faces(map, opaque, offset);
    local_buffer = std::vector<CubeVertex>(faces * INDICES_FACE_COUNT);
    auto v_it = local_buffer.begin();

    for(const auto& kv : map){
        const glm::ivec3& e{kv.first};
        const TileBlock& tileBlock{kv.second};

        v_it = generate_geometry(opaque, v_it, e, highest, e - offset, tileBlock);
    }
}

void Chunk::populate_opaque(const WorkerItem &wi, const glm::ivec3 &o, const std::vector<bool>& opaque, const std::vector<char>& highest) const {
    for(const auto& blockmaps_row : wi.block_maps){
        for(const auto bm : blockmaps_row){
            if(!bm){
                continue;
            }
            // kv is key-value (position-item)
            for(const auto& kv : *bm){
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

void Chunk::generate_buffer() {
    gpu_buffer.store_data(local_buffer);
}

void Chunk::generate_chunk() {
    WorkerItem wi{pq};

    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            const Chunk& other = *this;
            if(dq || dp){
                other = model.get_chunk_at(pq + glm::ivec2{dp, dq});
            }
            wi.block_maps[dp + 1][dq + 1] = (other) ? other.blockMap : nullptr;
        }
    }

    compute_chunk(wi);
    generate_buffer();
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

void Chunk::set_block(const glm::ivec3& position, const TileBlock& w) {
    blockMap.set_tileBlock(position, w);
}
