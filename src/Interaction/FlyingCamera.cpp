//
// Created by ultimatenick on 07/10/21.
//

#include "FlyingCamera.hpp"
#include "gtx/rotate_vector.hpp"
#include "Geometry/Chunk.hpp"

FlyingCamera::FlyingCamera(const glm::vec3 &camPos, const glm::vec2& orientation, float camSpeed) : pos{camPos}, speed{camSpeed} {
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

void FlyingCamera::shiftRight(float weight) {
    pos += glm::normalize(glm::cross(getFrontVector(), up)) * speed * weight;
}

void FlyingCamera::shift_up(float weight) {
    pos.y += speed * weight;
}

void FlyingCamera::shiftFront(float weight) {
    // front norm is 1
    pos += getFrontVector() * speed * weight;
}

glm::ivec2 FlyingCamera::getPq() const{
    return Chunk::chunked(pos);
}

glm::vec3 FlyingCamera::getFrontVector() const {
    return glm::normalize(glm::vec3{
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch)),
        glm::cos(glm::radians(yaw)) * glm::sin(glm::radians(pitch))
    });
}
