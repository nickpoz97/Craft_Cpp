//
// Created by ultimatenick on 07/10/21.
//

#include "FlyingCamera.hpp"
#include "gtx/rotate_vector.hpp"
#include "Geometry/Chunk.hpp"

FlyingCamera::FlyingCamera(const glm::vec3 &camPos, const glm::vec2& &orientation) : pos{camPos} {
    rotate(orientation.x, orientation.y);
}

void FlyingCamera::rotate(float yawOffset, float pitchOffset) {
    pitch = glm::clamp(pitch + pitchOffset, -89.0f, 89.0f);
    yaw += yawOffset;
    yaw = (static_cast<int>(yaw) % 360) + (yaw - static_cast<int>(yaw));
}

void FlyingCamera::rotate(const glm::vec2& offset){
    rotate(offset.x, offset.y);
}

void FlyingCamera::shift(const glm::vec3 &offset) {
    pos += offset;
}

glm::mat4 FlyingCamera::getViewMatrix() const{
    return glm::lookAt(pos, pos + getFrontVector(), up);
}

void FlyingCamera::shiftRight(float amount) {
    pos += glm::normalize(glm::cross(getFrontVector(), up)) * amount;
}

void FlyingCamera::shift_up(float amount) {
    pos.y += amount;
}

void FlyingCamera::shift_front(float amount) {
    // front norm is 1
    pos += getFrontVector() * amount;
}

glm::vec2 FlyingCamera::getPq() const{
    return Chunk::chunked(pos);
}

glm::vec3 FlyingCamera::getFrontVector() const {
    return {
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::cos(glm::radians(yaw)) * glm::sin(glm::radians(pitch))
    };
}
