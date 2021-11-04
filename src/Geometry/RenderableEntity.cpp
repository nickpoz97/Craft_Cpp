//
// Created by ultimatenick on 03/09/21.
//

#include "Geometry/RenderableEntity.hpp"
#include "Interaction/GameView.hpp"

namespace CraftCpp {
template<typename VertexType>
RenderableEntity<VertexType>::RenderableEntity() : gpu_buffer{GameView::isInstantiated()} {}

template<typename VertexType>
RenderableEntity<VertexType>::RenderableEntity(const std::vector<VertexType> &local_buffer) :
        RenderableEntity{} {
    if (!local_buffer.empty() && GameView::isInstantiated()) {
        gpu_buffer.storeData(local_buffer);
    }
}

template<typename VertexType>
void RenderableEntity<VertexType>::renderObject() const {
    gpu_buffer.drawTriangles();
}

template<typename VertexType>
void RenderableEntity<VertexType>::renderLines() const {
    gpu_buffer.drawLines();
}

template<typename VertexType>
void RenderableEntity<VertexType>::update_buffer(const std::vector<VertexType> &local_buffer) const {
    gpu_buffer.storeData(local_buffer);
}

template<typename VertexType>
RenderableEntity<VertexType>::RenderableEntity(RenderableEntity &&other) noexcept :
        gpu_buffer{std::move(other.gpu_buffer)} {}

template<typename VertexType>
RenderableEntity<VertexType> &RenderableEntity<VertexType>::operator=(RenderableEntity &&other) noexcept {
    gpu_buffer = std::move(other.gpu_buffer);
    return *this;
}

template
class RenderableEntity<Standard3DVertex>;

template
class RenderableEntity<Uv3DVertex>;

template
class RenderableEntity<NormalVertex>;

template
class RenderableEntity<CubeVertex>;

template
class RenderableEntity<Standard2DVertex>;

template
class RenderableEntity<Uv2DVertex>;
}