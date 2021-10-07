//
// Created by ultimatenick on 15/09/21.
//

#define GLFW_INCLUDE_NONE

#include <iostream>

#include "trigonometric.hpp"
#include "gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GameView.hpp"
#include "../Rendering/GLError.hpp"

int GameView::compute_scale_factor(int width, int height) {
    int result = width / height;
    result = glm::max(1, result);
    result = glm::min(2, result);
    return result;
}

int GameView::get_width() const {
    return width;
}

int GameView::get_height() const {
    return height;
}

int GameView::get_scale() const {
    return scale;
}

void GameView::update() {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    scale = compute_scale_factor(width, height);
}

void GameView::set_fov(int fov_degrees) {
    fov = fov_degrees;
}

void GameView::set_ortho(int ortho_size) {
    ortho = ortho_size;
}

int GameView::get_fov() const {
    return fov;
}

GameView::GameView(int width, int height, float fov, int ortho, bool is_fullscreen) :
width{width},
height{height},
fov{fov},
ortho{ortho},
scale{compute_scale_factor(width, height)}{
    if(!glfwInit()){
        std::cerr << "glfw not initialized" << '\n';
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = create_window(is_fullscreen);
    if(!window){
        std::cerr << "window not created" << '\n';
        return;
    }

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(VSYNC);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "glad not initialized" << '\n';
        return;
    }

    glfwSwapInterval(VSYNC);

    initialized = true;
}

GLFWwindow *GameView::create_window(bool is_fullscreen) {
    GLFWmonitor *monitor = nullptr;
    if(is_fullscreen){
        int mode_count;
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *modes = glfwGetVideoModes(monitor, &mode_count);
        width = modes[mode_count - 1].width;
        height = modes[mode_count - 1].height;
    }

    return glfwCreateWindow(width, height, "Craft Cpp", monitor, nullptr);
}

GLFWwindow *GameView::getWindow() {
    return window;
}

glm::mat4 GameView::get_proj_matrix(GameView::ProjType pt) const {
    switch (pt) {
        case ProjType::PERSP:
            return glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far);
        case ProjType::ORTHO_3D:
            return glm::ortho(-ortho * get_ratio() ,ortho * get_ratio(),
                              static_cast<float>(-ortho),static_cast<float>(+ortho),
                              -z_far, z_far);
        case ProjType::UI:
            return glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
        case ProjType::ITEM:
            return glm::ortho(-item_box_side() * get_ratio(), item_box_side() * get_ratio(),
                              -item_box_side(), item_box_side(), -1.0f, 1.0f);
    }
}

int GameView::get_ortho() const {
    return ortho;
}

float GameView::get_ratio() const{
    return width/height;
}

bool GameView::isInitialized() {
    return initialized;
}

GameView::~GameView() {
    glfwTerminate();
}

float GameView::item_box_side() const {
    float size = 64 * scale;
    return height / size / 2;
}
