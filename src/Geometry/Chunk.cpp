//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>
#include <iostream>

#include "Chunk.hpp"
#include "noise.hpp"
#include "CubicObject.hpp"

Chunk::Chunk(const glm::ivec2 &pq_coordinates, bool init) : block_map{}, pq{pq_coordinates}, SuperClass{},
    xz_boundaries{{
        {get_min_xz(pq_coordinates)[0], get_min_xz(pq_coordinates)[1]},
        {get_max_xz(pq_coordinates)[0], get_min_xz(pq_coordinates)[1]},
        {get_min_xz(pq_coordinates)[0], get_max_xz(pq_coordinates)[1]},
        {get_max_xz(pq_coordinates)[0], get_max_xz(pq_coordinates)[1]}
    }}
{
    if(init){ init_chunk();}
};

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
    return TileBlock(block_map.at(block_pos));
}

Chunk::operator bool() const {
    // check if Chunk contains at least 1 block
    return !block_map.empty();
}

Chunk::BufferType Chunk::compute_chunk_geometry(const std::array<const Chunk*, 6> &np) const {
    opaque_matrix_type opaque{};
    height_matrix_type highest{};

    // offset is the beginning of the chunk
    glm::ivec3 offset{
        pq.x * Chunk::SIZE - Chunk::SIZE - 1,
        -1,
        pq.y * Chunk::SIZE - Chunk::SIZE - 1
    };

    // this matrices store info about visibility
    populate_opaque_and_height_matrix(np, offset, opaque, highest);

    int n_faces = count_exposed_faces(block_map, opaque, offset);
    // each visible face has INDICES_FACE_COUNT indices that represent the triangle
    BufferType local_buffer = std::vector<CubeVertex>(n_faces * INDICES_FACE_COUNT);
    auto v_it = local_buffer.begin();

    for(const auto& kv : block_map){
        const glm::ivec3& block_pos{kv.first};
        const TileBlock& tileBlock{kv.second};

        // generate geometry of actual block (value returned by function is first free position in buffer)
        v_it = generate_block_geometry(opaque, v_it, block_pos, highest, block_pos - offset, tileBlock);
    }

    return local_buffer;
}

void Chunk::populate_opaque_and_height_matrix(const std::array<const Chunk*, 6> &np,
                                              const glm::ivec3 &offset,
                                              opaque_matrix_type &opaque,
                                              height_matrix_type &highest) const{
    // analyze neighbors maps
    for(const auto* neighbor_ref : np){
        // check if neighbor is not present or not instantiated
        if(!neighbor_ref || get_chunk_distance(*this, *neighbor_ref) > 1){
            continue;
        }
        // kv is key-value (position-item)
        for(const auto& kv : neighbor_ref->block_map){
            const glm::ivec3& block_pos{kv.first};
            const TileBlock& tileBlock{kv.second};
            // v is the pos relative to the beginning of the chunk (o)
            auto v = block_pos - offset;

#ifdef DEBUG
            //assert(opaque[v.x][v.y][v.z] == 0);
#endif
            opaque[v.x][v.y][v.z] = !tileBlock.is_transparent();
            if (opaque[v.x][v.y][v.z]) {
                // update highest block in the xz position
                highest[v.x][v.z] = glm::max(highest[v.x][v.z], static_cast<char>(v.y));
            }
        }
    }
}

int Chunk::count_exposed_faces(const MyMap& map, const opaque_matrix_type &opaque, const glm::ivec3& offset) const {
    int miny = 256;
    int maxy = 0;
    int n_faces = 0;

    for(const auto& kv : map) {
        glm::ivec3 abs_pos{kv.first};
        TileBlock tileBlock{kv.second};
        /*if(tileBlock.is_empty()){
            continue;
        }*/
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
#ifdef DEBUG
        assert(total >= 0 && total <= 6);
#endif
        if(total == 0){
            continue;
        }
        if(tileBlock.is_plant()){
            total = 4;
        }

        n_faces += total;
    }
    return n_faces;
}

Chunk::BufferType::iterator Chunk::generate_block_geometry(const opaque_matrix_type &opaque,
                                                                       BufferType::iterator vertex_it,
                                                                       const glm::ivec3& block_abs_pos,
                                                                       const height_matrix_type &highest,
                                                                       const glm::ivec3& v,
                                                                       TileBlock w) const {
    std::array<bool, 6> f{
            !opaque[v.x - 1][v.y][v.z],
            !opaque[v.x + 1][v.y][v.z],
            !opaque[v.x][v.y + 1][v.z],
            !opaque[v.x][v.y - 1][v.z] && (block_abs_pos.y > 0), // no underground
            !opaque[v.x][v.y][v.z - 1],
            !opaque[v.x][v.y][v.z + 1]
    };
#ifdef DEBUG
    auto begin_it = vertex_it;
#endif

    if(w.is_plant()){
        Plant plant{w.get_index(), {1, 1, 1, 1}, block_abs_pos, simplex2(block_abs_pos.x, block_abs_pos.z, 4, 0.5, 2) * 360, vertex_it};
#ifdef DEBUG
        assert(begin_it + 4 * INDICES_FACE_COUNT == plant.end());
#endif
        return plant.end();
    }

    else /*if(!w.is_empty())*/{
        // TODO check vertex_it
        Cube cube{w.get_index(), f, block_abs_pos, vertex_it};
#ifdef DEBUG
        int visible_faces = std::accumulate(f.begin(), f.end(), 0);
        assert(begin_it + visible_faces * INDICES_FACE_COUNT == cube.end());
#endif
        return cube.end();
    }
    /*else{
        return vertex_it;
    }*/
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

void Chunk::update_buffer(const std::array<const Chunk*, 6> &np) const {
    SuperClass::update_buffer(compute_chunk_geometry(np));
    dirty = false;
}

bool Chunk::is_dirty() const {
    return dirty;
}

void Chunk::generate_blockmap() {
    const int pad = 1;
    // for each block xz position
    for(int dx{-pad}; dx < Chunk::SIZE + pad ; dx++){
        for(int dz{-pad}; dz < Chunk::SIZE + pad ; dz++){
            int on_edge_flag = (dx < 0 || dz < 0 || dx >= Chunk::SIZE || dz >= Chunk::SIZE);
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
                block_map[{x, y, z}] = static_cast<BlockType>(w * !on_edge_flag);
            }
            if(w == BlockType::GRASS && !on_edge_flag) {
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
                            x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                    {
                        block_map[{x, y, z}] = BlockType::CLOUD;
                    }
                }
            }
        }
    }
    dirty = true;
}

bool Chunk::is_visible(const glm::mat4 &viewproj) const {
    /*glm::ivec3 result{};

    for(const auto& p : get_chunk_boundaries()){
        glm::vec4 clip_point = viewproj * glm::vec4{p, 1};
        clip_point /= clip_point.w;

        glm::ivec3 test_vector = glm::clamp(glm::ivec3{clip_point}, {-1, -1, -1}, {1, 1, 1});
        if(test_vector == glm::ivec3{}){
            return true;
        }
        result += test_vector;
    }
    return ((result.x == 0 || result.y == 0 || result.z == 0)) && glm::abs(result.z) < 8)*/;
    bool in{}, out{};
    for(const auto& p : xz_boundaries){
        glm::vec4 clip_point = viewproj * glm::vec4{p[0], 0, p[1], 1};
        clip_point = glm::abs(clip_point) / clip_point.w;

        bool is_in = clip_point.x < 1 && clip_point.z < 1;
        if(is_in){
            return true;
        }
    }
    return false;
}

std::array<glm::ivec3, 8> Chunk::get_chunk_boundaries() const {
    std::array<glm::ivec3, 8> boundaries{};
    auto it{boundaries.begin()};

    for(const auto& p : xz_boundaries){
        *(it++) = glm::vec3{p[0], 0, p[1]};
    }
    for(const auto& p : xz_boundaries){
        *(it++) = glm::vec3{p[0], getHighestBlock(), p[1]};
    }

    return boundaries;
}

void Chunk::set_block(const glm::ivec3& position, BlockType w) {
    block_map[position] = w;
    dirty = true;
}

int get_chunk_distance(const Chunk &c1, const Chunk &c2) {
    glm::ivec2 delta = c1.pq - c2.pq;
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

void Chunk::render_object(const std::array<const Chunk*, 6>& neighbors_refs) const{
    if(dirty && !block_map.empty()){
        update_buffer(neighbors_refs);
    }
    dirty = false;
    SuperClass ::render_object();
}

void Chunk::init_chunk() {
    init_chunk_threads.emplace_back(&Chunk::generate_blockmap, this);
}

void Chunk::wait_threads() {
    while(!init_chunk_threads.empty()) {
        std::thread &t = init_chunk_threads.front();
#ifdef DEBUG
        std::cout << t.get_id() << '\t';
        std::thread::id id{};
#endif
        t.join();
        init_chunk_threads.pop_front();
    }
}

glm::ivec2 Chunk::get_min_xz(const glm::ivec2& pq) {
    return pq * SIZE - 1;
}

glm::ivec2 Chunk::get_max_xz(const glm::ivec2& pq) {
    return get_min_xz(pq) + SIZE;
}
