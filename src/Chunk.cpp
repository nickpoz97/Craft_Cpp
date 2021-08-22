//
// Created by ultimatenick on 10/08/21.
//

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
