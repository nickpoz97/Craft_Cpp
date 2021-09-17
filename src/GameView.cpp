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

int GameView::compute_scale_factor(int width, int height) {
    int result = width / height;
    result = glm::max(1, result);
    result = glm::min(2, result);
    return result;
}

void GameView::update_all_proj_matrices() {
    update_ortho_proj_matrix();
    update_persp_proj_matrix();
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

void GameView::update_window() {
    glfwGetFramebufferSize(window, &width, &height);
    scale = compute_scale_factor(width, height);
    glViewport(0, 0, width, height);
    update_all_proj_matrices();
}

void GameView::set_fov(int fov_degrees) {
    fov = fov_degrees;
    update_persp_proj_matrix();
}

void GameView::update_persp_proj_matrix() {
    persp_proj = glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far);
}

void GameView::update_ortho_proj_matrix() {
    float ratio = width / height;

    ortho_proj_2d = glm::ortho(0, width, 0, height, -1, 1);
    ortho_proj_3d = glm::ortho(-ortho * ratio,ortho * ratio,
                               static_cast<float>(-ortho),static_cast<float>(+ortho),
                               -z_far, z_far);

    float size = 64 * scale;
    float box = height / size * 2;
    auto get_offset = [size](int axis){return 1 - size / axis * 2;};
    glm::vec2 offset{get_offset(width), get_offset(height)};

    ortho_proj_item = glm::ortho(-box * ratio, box * ratio, -box, box, -1.0f, 1.0f);
}

void GameView::set_ortho(int ortho_size) {
    ortho = ortho_size;
    update_ortho_proj_matrix();
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
    window = create_window(is_fullscreen);
    if(!window){
        std::cerr << "window not created" << '\n';
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(VSYNC);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "glad not initialized" << '\n';
        return;
    }

    update_all_proj_matrices();
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

GLFWwindow *GameView::get_window() const {
    return window;
}

glm::mat4 GameView::get_proj_matrix(GameView::proj_type pt) const {
    switch (pt) {
        case proj_type::PERSP:
            return persp_proj;
        case proj_type::ORTHO_3D:
            return ortho_proj_3d;
        case proj_type::TEXT:
            return ortho_proj_2d;
        case proj_type::ITEM:
            return ortho_proj_item;
    }
}

int GameView::get_ortho() const {
    return ortho;
}

float GameView::get_ratio() const{
    return width/height;
}

bool GameView::is_initialized() const {
    return initialized;
}

GameView::~GameView() {
    glfwTerminate();
}



