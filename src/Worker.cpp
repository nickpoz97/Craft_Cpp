//
// Created by ultimatenick on 10/08/21.
//

#include <noise.h>
#include "Worker.hpp"

bool WorkerItem::has_light() const{
    /*for(const auto& map_vector : light_maps){
        for(const BlockMap& lm : map_vector){
            if(!lm.empty()){
                return true;
            }
        }
    }*/
    return false;
}

WorkerItem::WorkerItem(const Chunk& chunk) : pq_coordinates{chunk.getPq()}{
    auto& actual_block_map = *block_maps[1][1];

    const int pad = 1;
    for(int dx{-pad}; dx < Chunk::size ; dx++){
        for(int dz{-pad}; dz < Chunk::size ; dz++){
            bool on_edge_flag = (dx < 0 || dz < 0 || dx >= Chunk::size || dz >= Chunk::size);
            int x = pq_coordinates.x * Chunk::size + dx;
            int z = pq_coordinates.y * Chunk::size + dz;

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
                actual_block_map.set_tileBlock({x,y,z}, static_cast<int>(w) * on_edge_flag);
            }
            if(w == BlockType::GRASS){
                if(SHOW_PLANTS){
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        actual_block_map.set_tileBlock({x, h, z}, static_cast<int>(BlockType::TALL_GRASS) * on_edge_flag);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        // w_f max == 23
                        int w_f = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        actual_block_map.set_tileBlock({x, h, z}, w_f * on_edge_flag);
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
                                    actual_block_map.set_tileBlock({x + ox, y, z + oz}, BlockType::LEAVES);
                                }
                            }
                        }
                    }
                    for (int y = h; y < h + 7; y++) {
                        actual_block_map.set_tileBlock({x, y, z}, BlockType::WOOD);
                    }
                }
                if (SHOW_CLOUDS) {
                    for (int y = 64; y < 72; y++) {
                        if (simplex3(
                                x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                        {
                            actual_block_map.set_tileBlock({x, y, z}, 16);
                        }
                    }
                }
            }
        }
    }
}


