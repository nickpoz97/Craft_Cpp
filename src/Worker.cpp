//
// Created by ultimatenick on 10/08/21.
//

#include "Worker.hpp"

bool WorkerItem::has_light() const{
    for(const auto& map_vector : light_maps){
        for(const BlockMap& lm : map_vector){
            if(!lm.empty()){
                return true;
            }
        }
    }
    return false;
}
