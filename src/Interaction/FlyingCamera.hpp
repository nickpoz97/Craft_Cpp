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
    glm::vec3 front;
    static constexpr glm::vec3 up{0, 1, 0};

public:
    FlyingCamera(const glm::vec3& camPos, const glm::vec3& camFront);
    void rotate(float yaw, float pitch);

    void shift(const glm::vec3& offset);
    void shiftRight(float amount);
    void shift_up(float amount);
    void shift_front(float amount);
    glm::mat4 getViewMatrix();
    glm::vec2 getPq();
};


#endif //CPP_FLYINGCAMERA_HPP
