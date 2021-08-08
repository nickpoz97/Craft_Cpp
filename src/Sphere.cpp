//
// Created by ultimatenick on 08/08/21.
//

#include <cmath>

#include "Sphere.hpp"

decltype(Sphere::indices) Sphere::indices{{
    {4, 3, 0}, {1, 4, 0},
    {3, 4, 5}, {4, 1, 5},
    {0, 3, 2}, {0, 2, 1},
    {5, 2, 3}, {5, 1, 2}
}};
decltype(Sphere::positions) Sphere::positions{{
    { 0, 0,-1}, { 1, 0, 0},
    { 0,-1, 0}, {-1, 0, 0},
    { 0, 1, 0}, { 0, 0, 1}
}};
decltype(Sphere::uvs) Sphere::uvs{{
        {0, 0.5}, {0, 0.5},
        {0, 0}, {0, 0.5},
        {0, 1}, {0, 0.5}
}};

Sphere::Sphere(int r, int detail) : vertices(r * pow(detail,4)){
    for(int count = 0 ; count < detail ; count++){
        for(const auto& i : indices){
            for(int iv : i){
                vertices.push_back({{positions[iv], 1}, iv, uvs[iv]});
            }
        }
    }
}