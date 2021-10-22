//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_RENDERABLEENTITY_HPP
#define CPP_RENDERABLEENTITY_HPP


#include <vector>
#include <memory>
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "../Rendering/OpenglBuffer.hpp"
#include "TileBlock.hpp"

template<typename VertexType>
class RenderableEntity{
private:
    mutable OpenglBuffer<VertexType> gpu_buffer;
public:
    explicit RenderableEntity();
    explicit RenderableEntity(const std::vector<VertexType>& local_buffer);
    explicit RenderableEntity(const RenderableEntity& other) = delete;
    RenderableEntity(RenderableEntity&& other) noexcept ;
    RenderableEntity<VertexType>& operator= (const std::vector<VertexType>& local_buffer) = delete;
    RenderableEntity<VertexType>& operator= (RenderableEntity&& other) noexcept ;

    void update_buffer(const std::vector<VertexType>& local_buffer) const;
    void render_object() const;
    void render_lines() const;
};

#endif //CPP_RENDERABLEENTITY_HPP
