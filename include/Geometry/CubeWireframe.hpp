//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_CUBEWIREFRAME_HPP
#define CPP_CUBEWIREFRAME_HPP

#include <array>

#include "Geometry/CubicObject.hpp"

class CubeWireframe : public RenderableEntity<Standard3DVertex>{
private:
    using SuperClass = RenderableEntity<Standard3DVertex>;

    static constexpr int N_VERTICES = 8;
    static constexpr int N_INDICES = 24;
    static constexpr float N = 0.5;

    static std::array<glm::vec3, N_VERTICES> local_vertex_positions;
    static std::array<int, N_INDICES> indices;

    OpenglBuffer<Standard3DVertex> gpu_Buffer;
public:
    static std::vector<Standard3DVertex> generate_local_buffer(const glm::vec3 &position);
    explicit CubeWireframe(const glm::vec3 &position);
    void render_object() = delete;
};


#endif //CPP_CUBEWIREFRAME_HPP
