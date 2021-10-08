//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_FLYINGCAMERA_HPP
#define CPP_FLYINGCAMERA_HPP

#include "vec3.hpp"
#include "mat4x4.hpp"

class FlyingCamera {
private:
    glm::vec3 pos;
    float pitch{};
    float yaw{};
    float speed;

    static constexpr glm::vec3 up{0, 1, 0};
    [[nodiscard]] glm::vec3 getFrontVector() const;
public:
    FlyingCamera(const glm::vec3& camPos, const glm::vec2& orientation, float camSpeed = 0.05);
    FlyingCamera() = delete;
    FlyingCamera(const FlyingCamera& flyingCamera) = default;
    FlyingCamera(FlyingCamera&& flyingCamera) = default;

    FlyingCamera& operator=(const FlyingCamera& flyingCamera) = default;
    FlyingCamera& operator=(FlyingCamera&& flyingCamera) = default;

    void rotate(float yawOffset, float pitchOffset);
    void rotate(const glm::vec2& offset);

    void shift(const glm::vec3& offset);
    void shiftRight(float weight);
    void shift_up(float weight);
    void shiftFront(float weight);
    glm::mat4 getViewMatrix() const;
    glm::ivec2 getPq() const;
};


#endif //CPP_FLYINGCAMERA_HPP
