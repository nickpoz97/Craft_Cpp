//
// Created by ultimatenick on 10/08/21.
//

#include <numeric>

#include "Chunk.hpp"
#include "Model.hpp"
#include "CubicObject.hpp"

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

    glm::ivec3 offset{
        wi.pq_coordinates.x * Chunk::size - Chunk::size - 1,
        -1,
        wi.pq_coordinates.y * Chunk::size - Chunk::size - 1
    };

    bool has_light = wi.has_light();

    populate_opaque(wi, offset, opaque, highest);
    if(has_light){
        flood_fill(wi, offset, opaque, light);
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
            // kv is key-value (relative_position-item)
            for(const auto& kv : bm){
                // absolute position
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

void Chunk::generate_geometry(const std::vector<char> &opaque, const std::vector<char> &light,
                              const std::vector<char> &highest, const glm::vec3& v, Item w, int total) {
    std::array<char, 27> neighbors{};
    std::array<char, 27> lights{};
    std::array<char, 27> shades{};

    int index = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++, index++){
                neighbors[index] = opaque[XYZ(v.x + dx, v.y + dy, v.z + dz)];
                lights[index] = light[XYZ(v.x + dx, v.y + dy, v.z + dz)];
                shades[index] = 0;
                if (v.y + dy <= highest[XZ(v.x + dx, v.z + dz)]) {
                    for (int oy = 0; oy < 8; oy++) {
                        if (opaque[XYZ(v.x + dx, v.y + dy + oy, v.z + dz)]) {
                            shades[index] = 1.0 - oy * 0.125;
                            break;
                        }
                    }
                }
            }
        }
    }

    std::array<std::array<float,4>, 6> ao;
    std::array<std::array<float,4>, 6> _light;

    occlusion(neighbors, lights, shades, ao, _light);

    if(w.is_plant()){
        total = 4;
        float min_ao = 1;
        float max_light = 0;
        for (int a = 0; a < 6; a++) {
            for (int b = 0; b < 4; b++) {
                min_ao = glm::min(min_ao, ao[a][b]);
                max_light = glm::max(max_light, _light[a][b]);
            }
        }
    }


}

void Chunk::occlusion(const std::array<char, 27>& neighbors,
                      const std::array<char, 27>& lights,
                      const std::array<char, 27>& shades,
                      std::array<std::array<float,4>, 6>& ao,
                      std::array<std::array<float,4>, 6>& light){
    static const std::array<std::array<glm::vec3, 4>, 6> lookup3{{
        {{{0, 1, 3}, {2, 1, 5}, {6, 3, 7}, {8, 5, 7}}},
        {{{18, 19, 21}, {20, 19, 23}, {24, 21, 25}, {26, 23, 25}}},
        {{{6, 7, 15}, {8, 7, 17}, {24, 15, 25}, {26, 17, 25}}},
        {{{0, 1, 9}, {2, 1, 11}, {18, 9, 19}, {20, 11, 19}}},
        {{{0, 3, 9}, {6, 3, 15}, {18, 9, 21}, {24, 15, 21}}},
        {{{2, 5, 11}, {8, 5, 17}, {20, 11, 23}, {26, 17, 23}}}
    }};

    static const static const std::array<std::array<glm::vec4, 4>, 6> lookup4{{
        {{{0, 1, 3, 4}, {1, 2, 4, 5}, {3, 4, 6, 7}, {4, 5, 7, 8}}},
        {{{18, 19, 21, 22}, {19, 20, 22, 23}, {21, 22, 24, 25}, {22, 23, 25, 26}}},
        {{{6, 7, 15, 16}, {7, 8, 16, 17}, {15, 16, 24, 25}, {16, 17, 25, 26}}},
        {{{0, 1, 9, 10}, {1, 2, 10, 11}, {9, 10, 18, 19}, {10, 11, 19, 20}}},
        {{{0, 3, 9, 12}, {3, 6, 12, 15}, {9, 12, 18, 21}, {12, 15, 21, 24}}},
        {{{2, 5, 11, 14}, {5, 8, 14, 17}, {11, 14, 20, 23}, {14, 17, 23, 26}}}
    }};

    static const std::array<float,4> curve{0.0, 0.25, 0.5, 0.75};

    for(int i = 0 ; i < lookup3.size() ; i++){
        for(int j = 0 ; j < lookup3[0].size() ; j++){
            int corner = neighbors[lookup3[i][j].x];
            bool side1 = neighbors[lookup3[i][j].y];
            bool side2 = neighbors[lookup3[i][j].z];
            int value = (side1 && side2) ? 3 : corner + side1 + side2;
            float shade_sum = 0;
            float light_sum = 0;

            bool is_light = (lights[13] == 15);

            for (int k = 0; k < 4; k++) {
                shade_sum += shades[lookup4[i][j][k]];
                light_sum += lights[lookup4[i][j][k]];
            }

            if (is_light) {
                light_sum = 15 * 4 * 10;
            }
            float total = curve[value] + shade_sum / 4.0;
            ao[i][j] = glm::min(total, 1.0f);
            light[i][j] = light_sum / 15.0 / 4.0;
        }
    }
}
