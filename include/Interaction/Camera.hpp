//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_CAMERA_HPP
#define CPP_CAMERA_HPP

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace CraftCpp {
class Camera {
private:
    glm::vec3 pos;

    float pitch{};
    float yaw{};
    glm::vec3 direction{};
    float speed;

    [[nodiscard]] glm::vec3 getFrontVector() const;

public:
    Camera(const glm::vec3 &camPos, const glm::vec2 &rotation, float camSpeed = 10);

    Camera() = delete;

    Camera(const Camera &flyingCamera) = default;

    Camera(Camera &&flyingCamera) = default;

    Camera &operator=(const Camera &flyingCamera) = default;

    Camera &operator=(Camera &&flyingCamera) = default;

    void rotate(float yawOffset, float pitchOffset);

    void rotate(const glm::vec2 &offset);

    void shift(const glm::vec3 &offset);

    void shiftRight(float weight = 1.0f);

    void shiftUp(float weight = 1.0f);

    void shiftFront(float weight = 1.0f);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::ivec2 getPq() const;

    void printCameraInfo() const;

    [[nodiscard]] const glm::vec3 &getPos() const;

    const glm::vec3 &getDirection() const;

    friend class Scene;

    float getSpeed() const;

    static constexpr glm::vec3 up{0, 1, 0};

    [[nodiscard]] float getYaw() const;
    [[nodiscard]] float getPitch() const;
};
}

#endif //CPP_CAMERA_HPP
