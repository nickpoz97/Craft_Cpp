//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>

#include "Chunk.hpp"
#include "Model.hpp"

void Chunk::draw() {
    //TODO implement method using CubicObject draw function
}

constexpr int Chunk::getSize() {
    return size;
}

int Chunk::getMinY() {
    return min_y;
}

int Chunk::getMaxY() {
    return max_y;
}

Item Chunk::getHighestBlock() const{
    for(auto rit{map.rbegin()} ; rit != map.rend() ; ++rit){
        if((rit->second).is_obstacle())
            return rit->second;
    }
    return Item{};
}

const glm::ivec2 &Chunk::getPq() const {
    return pq;
}

Item Chunk::get_block(const glm::ivec3& block_pos) const{
    return map.at(block_pos);
}

bool Chunk::operator!() const {
    return map.empty();
}

void Chunk::gen_sign_buffer() {
    int n_chars = 0;

    for(const auto& sign : sign_list){
        n_chars += sign.get_text().length();
    }
    std::vector<std::array<Uv3DVertex, Character3D::n_vertices>> tmp_buffer(n_chars);
    auto sign_it = sign_list.begin();

    // fill buffer
    for(const auto& c3d_vertices : tmp_buffer){
        std::copy(sign_it->get_chars().begin(), sign_it->get_chars().end(), c3d_vertices.begin());
        ++sign_it;
    }

    // TODO check if it is mandatory to delete buffer
    sign_buffer.store_data(sizeof(tmp_buffer), reinterpret_cast<GLfloat*>(tmp_buffer.data()));
}

// checks if one of the neighbors has lights
bool Chunk::has_lights() {
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
}

void Chunk::set_dirt() {
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
}

void Chunk::light_fill(std::vector<char>& opaque, std::vector<char>& light, const glm::vec3& v, int w, bool force) {

    int x = v.x;
    int y = v.y;
    int z = v.z;

    if (x + w < XZ_LO || z + w < XZ_LO) {
        return;
    }
    if (x - w > XZ_HI || z - w > XZ_HI) {
        return;
    }
    if (y < 0 || y >= Y_SIZE) {
        return;
    }
    if (light[XYZ(x, y, z)] >= w) {
        return;
    }
    if (!force && opaque[XYZ(x, y, z)]) {
        return;
    }
    light[XYZ(x, y, z)] = w--;
    light_fill(opaque, light, {x - 1, y, z}, w, 0);
    light_fill(opaque, light, {x + 1, y, z}, w, 0);
    light_fill(opaque, light, {x, y - 1, z}, w, 0);
    light_fill(opaque, light, {x, y + 1, z}, w, 0);
    light_fill(opaque, light, {x, y, z - 1}, w, 0);
    light_fill(opaque, light, {x, y, z + 1}, w, 0);
}

Chunk::Chunk(const WorkerItem &wi) : pq{wi.pq_coordinates}, model{model}{
    std::vector<char> opaque(XZ_SIZE * XZ_SIZE * Y_SIZE);
    std::vector<char> light(XZ_SIZE * XZ_SIZE * Y_SIZE);
    std::vector<char> highest(XZ_SIZE * XZ_SIZE);

    glm::ivec3 o{
        wi.pq_coordinates.x * Chunk::size - Chunk::size - 1,
        -1,
        wi.pq_coordinates.y * Chunk::size - Chunk::size - 1
    };

    bool has_light = wi.has_light();

    populate_opaque(wi, o, opaque, highest);
    if(has_light){
        flood_fill(wi, o, opaque, light);
    }

    auto map = wi.block_maps[1][1];
    faces += count_exposed_faces(map, opaque, o);
}

void Chunk::populate_opaque(const WorkerItem &wi, const glm::ivec3 &o, const std::vector<char>& opaque, const std::vector<char>& highest) const {
    for(const auto& blockmaps_row : wi.block_maps){
        for(const BlockMap& bm : blockmaps_row){
            if(bm.empty()){
                continue;
            }
            for(const auto& kv : bm){
                glm::ivec3 e{kv.first + bm.get_delta()};
                Item w = kv.second;
                if(w.is_empty()){
                    continue;
                }
                auto v = e - o;

                opaque[XYZ(v.x, v.y, v.z)] = !w.is_transparent();
                if (opaque[XYZ(v.x, v.y, v.z)]) {
                    highest[XZ(v.x, v.z)] = glm::max(highest[XZ(v.x, v.z)], v.y);
                }
            }
        }
    }
}

void Chunk::flood_fill(const WorkerItem& wi, const glm::ivec3& o, std::vector<char>& opaque, std::vector<char>& light) {
    for(const auto& lightmaps_row : wi.light_maps){
        for(const BlockMap& lm : lightmaps_row){
            if (lm.empty()) {
                continue;
            }
            for(const auto& kv : lm) {
                glm::ivec3 e{kv.first + lm.get_delta()};
                Item w = kv.second;
                auto v = e - o;
                light_fill(opaque, light, v, static_cast<int>(w), true);
            }
        }
    }
}

int Chunk::count_exposed_faces(const BlockMap& map, const std::vector<char>& opaque, const glm::ivec3& o) {
    int miny = 256;
    int maxy = 0;
    int faces = 0;

    for(const auto& kv : map) {
        glm::ivec3 e{kv.first + map.get_delta()};
        Item w = kv.second;
        if(w.is_empty()){
            continue;
        }
        auto v = e - o;

        std::array<int, 6> f{
            !opaque[XYZ(v.x - 1, v.y, v.z)],
            !opaque[XYZ(v.x + 1, v.y, v.z)],
            !opaque[XYZ(v.x, v.y + 1, v.z)],
            !opaque[XYZ(v.x, v.y - 1, v.z)] && (e.y > 0),
            !opaque[XYZ(v.x, v.y, v.z - 1)],
            !opaque[XYZ(v.x, v.y, v.z + 1)]
        };

        int total = std::accumulate(f.begin(), f.end(), 0);
        if(w.is_plant()){
            total = 4;
        }

        min_y = glm::min(min_y, e.y);
        max_y = glm::max(max_y, e.y);

        return total;
    }
}
