//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_RENDERABLEENTITY_HPP
#define CPP_RENDERABLEENTITY_HPP


#include <vector>
#include <memory>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Rendering/OpenglBuffer.hpp"
#include "Geometry/TileBlock.hpp"

namespace CraftCpp {

/**
 * @brief Class which manages allocation and rendering of UI and world objects (entities)
 * @tparam VertexType type of vertex from Vertex.hpp on which the entity have been built
 */
template<typename VertexType>
class RenderableEntity {
private:
    mutable OpenglBuffer<VertexType> gpu_buffer;
public:
    /// @brief allocates gpu buffer
    explicit RenderableEntity();

    /**
     * @brief allocates gpu buffer and initializes VBO with a local buffer
     * @param[in] local_buffer array of vertices
     */
    explicit RenderableEntity(const std::vector<VertexType> &local_buffer);

    explicit RenderableEntity(const RenderableEntity &other) = delete;

    /**
     * @brief moves ownership from another object
     * @param[in] other object to be moved
     */
    RenderableEntity(RenderableEntity &&other) noexcept;

    RenderableEntity<VertexType> &operator=(const std::vector<VertexType> &local_buffer) = delete;

    /**
     * @brief moves ownership from another object
     * @param[in] other object to be moved
     */
    RenderableEntity<VertexType> &operator=(RenderableEntity &&other) noexcept;

    /**
     * @brief copies a new array of vertices into gpu VBO
     * @param local_buffer
     */
    void updateBuffer(const std::vector<VertexType> &local_buffer) const;

    /**
     * @brief interprets vertex array on gpu as a sequence of triangles and draws that on rendering buffer
     * @warning subsequent calls overwrites buffer values
     */
    void renderObject() const;

    /**
     * @brief interprets vertex array on gpu as a sequence of lines and draws that on rendering buffer
     * @warning subsequent calls overwrites buffer values
     */
    void renderLines() const;
};
}
#endif //CPP_RENDERABLEENTITY_HPP
