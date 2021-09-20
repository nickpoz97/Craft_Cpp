//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_RENDERABLEENTITY_HPP
#define CPP_RENDERABLEENTITY_HPP


#include <vector>
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "../Rendering/OpenglBuffer.hpp"
#include "TileBlock.hpp"

template<typename VertexType>
class RenderableEntity{
private:
    OpenglBuffer<VertexType> gpu_buffer{};
public:
    explicit RenderableEntity(const std::vector<VertexType>& local_buffer);
    explicit RenderableEntity() = default;
    //template<size_t n_values> explicit RenderableEntity(const std::array<VertexType, n_values>& local_buffer);
    void update_buffer(const std::vector<VertexType>& local_buffer);
    void render_object() const;
    void render_lines() const;
};

#endif //CPP_RENDERABLEENTITY_HPP
