#define GLFW_INCLUDE_NONE

#include <iostream>
#include <Interaction/Scene.hpp>

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    GameViewSettings gvs{{WINDOW_WIDTH, WINDOW_WIDTH}, INITIAL_FOV};
    ShaderNamesMap snm{};
    snm.emplace(ShaderName::BLOCK_SHADER, ShaderFilesPaths{
        "../data/shaders/block_vertex.glsl","../data/shaders/block_fragment.glsl"
    });

    Scene& scene{*Scene::setInstance(gvs, {0, 20, 0}, {0,-1,1}, snm)};
    if(scene.load_texture("../data/textures/texture.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }

    while(!glfwWindowShouldClose(GameView::getWindow())) {
        scene.loop();
    }
    Scene::clear();
    return 0;
}