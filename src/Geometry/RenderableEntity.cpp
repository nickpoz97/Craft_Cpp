//
// Created by ultimatenick on 03/09/21.
//

#include "RenderableEntity.hpp"

template<typename VertexType>
RenderableEntity<VertexType>::RenderableEntity(const std::vector<VertexType>& local_buffer) {
    if(!local_buffer.empty())
        gpu_buffer.store_data(local_buffer);
}

template<typename VertexType>
void RenderableEntity<VertexType>::render_object() const {
    gpu_buffer.draw_triangles();
}

template<typename VertexType>
void RenderableEntity<VertexType>::render_lines() const {
    gpu_buffer.draw_lines();
}

/*template<typename VertexType>
template<size_t n_values>
RenderableEntity<VertexType>::RenderableEntity(const std::array<VertexType, n_values> &local_buffer) {
    gpu_buffer.store_data(local_buffer);
}*/

template<typename VertexType>
void RenderableEntity<VertexType>::update_buffer(const std::vector<VertexType> &local_buffer) const {
    gpu_buffer.store_data(local_buffer);
}

template class RenderableEntity<Standard3DVertex>;
template class RenderableEntity<Uv3DVertex>;
template class RenderableEntity<NormalVertex>;
template class RenderableEntity<CubeVertex>;
template class RenderableEntity<Standard2DVertex>;
template class RenderableEntity<Uv2DVertex>;
