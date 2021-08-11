//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_CUBEWIREFRAME_HPP
#define CPP_CUBEWIREFRAME_HPP

#include <array>
#include "CubicObject.hpp"

class CubeWireframe{
private:
    static constexpr int N_VERTICES = 8;
    static constexpr int N_INDICES = 24;
    static constexpr float N = 0.5;

    static std::array<glm::vec3, N_VERTICES> local_vertex_positions;
    static std::array<int, N_INDICES> indices;

    std::vector<glm::vec3> vertices;
public:
    CubeWireframe();
};


#endif //CPP_CUBEWIREFRAME_HPP
