//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_CAMERA_HPP
#define CPP_CAMERA_HPP

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace CraftCpp {

/**
* @brief First person view of the 3D model
*/
class Camera {
private:
    glm::vec3 pos;

    float pitch{};
    float yaw{};
    glm::vec3 direction{};
    float speed;

    [[nodiscard]] glm::vec3 getFrontVector() const;

public:
    /**
    * @brief Instantiate Camera with a specific status
    * @param camPos 3D world coordinates of the camera
    * @param rotation 2D vector consisting of 2 euler angles (yaw, pitch) that defines camera direction
    * @param camSpeed Camera object translation speed
    */
    Camera(const glm::vec3 &camPos, const glm::vec2 &rotation, float camSpeed = 10);
    Camera() = delete;
    /// @brief instantiate Camera using another Camera status
    /// @warning code with multiple Camera object has not been tested
    Camera(const Camera &flyingCamera) = default;
    /// @brief instantiate Camera using another Camera status
    Camera(Camera &&flyingCamera) = default;

    /// @warning code with multiple Camera object has not been tested
    Camera &operator=(const Camera &flyingCamera) = default;

    Camera &operator=(Camera &&flyingCamera) = default;

    void rotate(float yawOffset, float pitchOffset);

    void rotate(const glm::vec2 &offset);

    void shift(const glm::vec3 &offset);

    void shiftRight(float weight = 1.0f);

    /**
     * @brief translates the camera upward (following y-axis) by cameraSpeed quantity
     * @param weight multiplicative factor
     * @note weight should be the time elapsed between actual frame and the former
     */
    void shiftUp(float weight = 1.0f);

    void shiftFront(float weight = 1.0f);

    /** @defgroup getters Attribute Getters
     *
     * get attributes values or infos about Camera object
     * @{
     */
     /// @returns view matrix for world_space -> view_space transition
     /// @note it depends on the actual position and direction
    [[nodiscard]] glm::mat4 getViewMatrix() const;
     /// @returns Camera object position in Chunk coordinates
     /// @note it depends on Chunk::SIZE
    [[nodiscard]] glm::ivec2 getPq() const;
    [[nodiscard]] const glm::vec3 &getPos() const;
    [[nodiscard]] const glm::vec3 &getDirection() const;
    [[nodiscard]] float getSpeed() const;
    /**@}*/

    friend class Scene;

    ///@brief vector representing what is the "up direction" of the 3D world
    static constexpr glm::vec3 up{0, 1, 0};

    [[nodiscard]] float getYaw() const;
    [[nodiscard]] float getPitch() const;
};
}

#endif //CPP_CAMERA_HPP
