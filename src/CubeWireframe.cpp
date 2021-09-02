//
// Created by ultimatenick on 11/08/21.
//

#include "CubeWireframe.hpp"

decltype(CubeWireframe::local_vertex_positions) CubeWireframe::local_vertex_positions{{
    {-1, -1, -1},
    {-1, -1, +1},
    {-1, +1, -1},
    {-1, +1, +1},
    {+1, -1, -1},
    {+1, -1, +1},
    {+1, +1, -1},
    {+1, +1, +1}
}
};

decltype(CubeWireframe::indices) CubeWireframe::indices{
    {
        0, 1, 0, 2, 0, 4, 1, 3,
        1, 5, 2, 3, 2, 6, 3, 7,
        4, 5, 4, 6, 5, 7, 6, 7
    }
};

CubeWireframe::CubeWireframe(const glm::vec3 &position) {
    std::vector<Standard3DVertex> vertices(N_INDICES);

    decltype(indices)::iterator ii;
    decltype(vertices)::iterator iv;

    for(ii = indices.begin(), iv = vertices.begin() ; ii != indices.end() ; ++ii, ++iv){
        iv->position = position + local_vertex_positions[*ii] * N;
    }

    gpu_Buffer.store_data(vertices);
}

void CubeWireframe::render() const {
    gpu_Buffer.draw_lines()
}

