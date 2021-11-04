//
// Created by ultimatenick on 15/09/21.
//

#define GLFW_INCLUDE_NONE

#include <iostream>
#include "Interaction/GameView.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Rendering/OpenGlExternal.hpp"

namespace CraftCpp {
int GameView::computeScaleFactor(int width, int height) {
    int result = width / height;
    result = glm::max(1, result);
    result = glm::min(2, result);
    return result;
}

int GameView::getWidth() const {
    return width;
}

int GameView::getHeight() const {
    return height;
}

int GameView::getScale() const {
    return scale;
}

void GameView::update() {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    scale = computeScaleFactor(width, height);
}

void GameView::setFov(int fov_degrees) {
    fov = fov_degrees;
}

void GameView::setOrtho(int ortho_size) {
    ortho = ortho_size;
}

int GameView::getFov() const {
    return fov;
}

GameView::GameView(int width, int height, float fov, int ortho, bool isFullscreen) :
        width{width},
        height{height},
        fov{fov},
        ortho{ortho},
        scale{computeScaleFactor(width, height)} {
    if (!glfwInit()) {
        std::cerr << "glfw not initialized" << '\n';
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = createWindow(isFullscreen);
    if (!window) {
        std::cerr << "window not created" << '\n';
        return;
    }

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(VSYNC);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "glad not initialized" << '\n';
        return;
    }

    glfwSwapInterval(VSYNC);
}

GLFWwindow *GameView::createWindow(bool is_fullscreen) {
    GLFWmonitor *monitor = nullptr;
    if (is_fullscreen) {
        int mode_count;
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *modes = glfwGetVideoModes(monitor, &mode_count);
        width = modes[mode_count - 1].width;
        height = modes[mode_count - 1].height;
    }

    return glfwCreateWindow(width, height, "Craft Cpp", monitor, nullptr);
}

GLFWwindow *GameView::getWindow() {
    return actualInstance ? actualInstance->window : nullptr;
}

glm::mat4 GameView::getProjMatrix(GameView::ProjType pt) const {
    switch (pt) {
        case ProjType::PERSP:
            return glm::perspective(glm::radians(fov), static_cast<float>(width) / height, z_near, z_far);
        case ProjType::UI:
            return glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
        case ProjType::ITEM:
            return glm::ortho(-item_box_side() * getRatio(), item_box_side() * getRatio(),
                              -item_box_side(), item_box_side(), -1.0f, 1.0f);
        // Ortho 3D
        default:
            return glm::ortho(-ortho * getRatio(), ortho * getRatio(),
                              static_cast<float>(-ortho), static_cast<float>(+ortho),
                              -z_far, z_far);
    }
}

int GameView::getOrtho() const {
    return ortho;
}

float GameView::getRatio() const {
    return static_cast<float>(width) / height;
}

bool GameView::isInstantiated() {
    return actualInstance;
}

GameView::~GameView() {
    actualInstance = nullptr;
    glfwTerminate();
#ifndef NDEBUG
    std::cout << "GLFW terminated\n";
#endif
}

float GameView::item_box_side() const {
    float size = 64 * scale;
    return height / size / 2;
}

GameView *GameView::getActualInstance() {
    return actualInstance;
}

std::unique_ptr<GameView> GameView::setInstance(int width, int height, float fov, int ortho, bool isFullscreen) {
    if (!isInstantiated()) {
        actualInstance = new GameView{width, height, fov, ortho, isFullscreen};
        glfwSetFramebufferSizeCallback(
            getWindow(),
            [](GLFWwindow *window, int width, int height) { actualInstance->update(); }
        );
    }
    return std::unique_ptr<GameView>{actualInstance};
}
}