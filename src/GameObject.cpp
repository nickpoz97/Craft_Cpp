//
// Created by ultimatenick on 03/09/21.
//

#include "GameObject.hpp"

template<typename VertexType>
GameObject<VertexType>::GameObject(const std::vector<VertexType>& local_buffer) {
    if(!local_buffer.empty())
        gpu_buffer.store_data(local_buffer);
}

template<typename VertexType>
void GameObject<VertexType>::render_object() const {
    gpu_buffer.draw_triangles();
}

template<typename VertexType>
void GameObject<VertexType>::render_lines() const {
    gpu_buffer.draw_lines();
}

template<typename VertexType>
template<size_t n_values>
GameObject<VertexType>::GameObject(const std::array<VertexType, n_values> &local_buffer) {
    gpu_buffer.store_data(local_buffer);
}

template<typename VertexType>
void GameObject<VertexType>::update_buffer(const std::vector<VertexType> &local_buffer) {
    gpu_buffer.store_data(local_buffer);
}

template class GameObject<Standard3DVertex>;
template class GameObject<Uv3DVertex>;
template class GameObject<NormalVertex>;
template class GameObject<CubeVertex>;
template class GameObject<Standard2DVertex>;
template class GameObject<Uv2DVertex>;
