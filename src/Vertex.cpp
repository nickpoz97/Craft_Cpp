//
// Created by ultimatenick on 08/08/21.
//

#include "Vertex.hpp"

Face CubeVertex::getFace() const {
    return face;
}

void CubeVertex::setFace(Face face) {
    CubeVertex::face = face;
}

float CubeVertex::getAo() const {
    return ao;
}

void CubeVertex::setAo(float ao) {
    CubeVertex::ao = ao;
}

float CubeVertex::getLight() const {
    return light;
}

void CubeVertex::setLight(float light) {
    CubeVertex::light = light;
}
