//
// Created by ultimatenick on 07/10/21.
//

#include "Camera.hpp"
#include "gtx/rotate_vector.hpp"
#include "Geometry/Chunk.hpp"
#include "fmt/format.h"

Camera::Camera(const glm::vec3 &camPos, const glm::vec3& camDirection, float camSpeed) :
    pos{camPos},
    speed{camSpeed},
    direction{glm::normalize(camDirection)},
    pitch{glm::asin(direction.y)},
    yaw{glm::acos(direction.x + direction.z)}
    {
#ifndef NDEBUG
    printCameraInfo();
#endif
}

void Camera::rotate(float yawOffset, float pitchOffset) {
    pitch = glm::clamp(pitch + pitchOffset, -89.0f, 89.0f);
    yaw += yawOffset;
    yaw = (static_cast<int>(yaw) % 360) + (yaw - static_cast<int>(yaw));

    direction = glm::normalize(glm::vec3{
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    });
#ifdef PRINT_CAM_INFO
    printCameraInfo();
#endif
}

void Camera::rotate(const glm::vec2& offset){
    rotate(offset.x, offset.y);
}

void Camera::shift(const glm::vec3 &offset) {
    pos += offset;
}

glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(pos, pos + getFrontVector(), up);
}

void Camera::shiftRight(float weight) {
    pos += glm::normalize(glm::cross(getFrontVector(), up)) * speed * weight;
}

void Camera::shift_up(float weight) {
    pos.y += speed * weight;
}

void Camera::shiftFront(float weight) {
    // front norm is 1
    pos += getFrontVector() * speed * weight;
}

glm::ivec2 Camera::getPq() const{
    return Chunk::chunked(pos);
}

glm::vec3 Camera::getFrontVector() const {
    return direction;
}

void Camera::printCameraInfo() const{
    fmt::print("yaw: {}, pitch: {}, direction:({},{},{})\n",
               yaw, pitch, direction.x, direction.y, direction.z);
}

const glm::vec3 &Camera::getPos() const {
    return pos;
}
