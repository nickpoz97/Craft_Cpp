//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_GAMEOBJECT_HPP
#define CPP_GAMEOBJECT_HPP


#include <vector>
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "OpenglBuffer.hpp"
#include "TileBlock.hpp"

template<typename VertexType>
class GameObject{
private:
    OpenglBuffer<VertexType> gpu_buffer{};
public:
    explicit GameObject(const std::vector<VertexType>& local_buffer);
    template<size_t n_values> explicit GameObject(const std::array<VertexType, n_values>& local_buffer);
    void update_buffer(const std::vector<VertexType>& local_buffer);
    void render_object() const;
    void render_lines() const;
};

#endif //CPP_GAMEOBJECT_HPP
