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

    static constexpr glm::vec3 up{0, 1, 0};
    [[nodiscard]] glm::vec3 getFrontVector() const;
public:
    FlyingCamera(const glm::vec3& camPos, const glm::vec2& orientation);
    void rotate(float yawOffset, float pitchOffset);
    void rotate(const glm::vec2& offset);

    void shift(const glm::vec3& offset);
    void shiftRight(float amount);
    void shift_up(float amount);
    void shift_front(float amount);
    glm::mat4 getViewMatrix() const;
    glm::vec2 getPq() const;
};


#endif //CPP_FLYINGCAMERA_HPP
