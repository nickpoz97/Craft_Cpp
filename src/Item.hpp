//
// Created by ultimatenick on 03/09/21.
//

#ifndef CPP_ITEM_HPP
#define CPP_ITEM_HPP


#include "Vertex.hpp"
#include "GameObject.hpp"

class Item : public GameObject<CubeVertex>{
public:
    using SuperClass = GameObject<CubeVertex>;
    Item(TileBlock w);
private:
    std::vector<CubeVertex> gen_local_buffer(TileBlock w);
};


#endif //CPP_ITEM_HPP
