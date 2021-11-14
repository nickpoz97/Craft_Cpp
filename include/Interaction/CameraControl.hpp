//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_CAMERACONTROL_HPP
#define CPP_CAMERACONTROL_HPP

#include <memory>
#include <glm/vec2.hpp>

#include "Rendering/OpenGlExternal.hpp"
#include "Interaction/Camera.hpp"
#include "Interaction/GameView.hpp"

namespace CraftCpp {

/// @brief Singleton class that interfaces external input with a Camera object
class CameraControl {
private:
    static inline CameraControl* actualInstance{nullptr};
    glm::vec2 lastMousePos;
    Camera &flyingCamera;
    static constexpr float mouseSensitivity{0.1};
    double lastFrameTime{};

    bool cursorInvisible{true};
    void switchCursorStatus();

    CameraControl(Camera &flyingCamera, const glm::ivec2 &initialMousePos, GLFWwindow *window);
public:
    /**
     * @brief set the unique instance of CameraControl
     * @param[in] flyingCamera lvalue reference to the handled Camera object
     * @return reference and ownership of CameraControl object
     * @note calling this method while there is an instance does nothing and return a nullptr
     */
    static std::unique_ptr<CameraControl> setInstance(Camera &flyingCamera);
    /// @brief makes possible new GameView instance
    ~CameraControl();
    /**
     * @brief uses mouse 2d coordinates to rotate attached Camera object
     * @param[in] window this callback is set for GLFWwindow referenced by this pointer
     * @param[in] xpos mouse x coordinate
     * @param[in] ypos mouse y coordinate
     */
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    /// @brief handles camera movement
    void processKeyboardInput();
    /// @return true iff CameraControl singleton has already been instantiated
    static bool isInstantiated();
};
}

#endif //CPP_CAMERACONTROL_HPP
