//
// Created by ultimatenick on 07/10/21.
//

#include "FlyingCamera.hpp"
#include "gtx/rotate_vector.hpp"
#include "Geometry/Chunk.hpp"

FlyingCamera::FlyingCamera(const glm::vec3 &camPos, const glm::vec3 &camFront) : pos{camPos}, front{glm::normalize(camFront)} {}

void FlyingCamera::rotate(float yaw, float pitch) {
    front = glm::rotateY(glm::rotateX(front, pitch), yaw);
}

void FlyingCamera::shift(const glm::vec3 &offset) {
    pos += offset;
}

glm::mat4 FlyingCamera::getViewMatrix() {
    return glm::lookAt(pos, pos + front, up);
}

void FlyingCamera::shiftRight(float amount) {
    pos += glm::normalize(glm::cross(front, up)) * amount;
}

void FlyingCamera::shift_up(float amount) {
    pos.y += amount;
}

void FlyingCamera::shift_front(float amount) {
    // front norm is 1
    pos += front * amount;
}

glm::vec2 FlyingCamera::getPq() {
    return Chunk::chunked(pos);
}
