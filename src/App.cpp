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
    snm.emplace(ShaderName::TEXT_SHADER, ShaderFilesPaths{
            "../data/shaders/text_vertex.glsl","../data/shaders/text_fragment.glsl"
    });

    Scene& scene{*Scene::setInstance(gvs, {0, 20, 0}, {-90,-10}, snm)};
    if(scene.load_texture("../data/textures/texture.png", TextureName::GENERAL, 0) != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(scene.load_texture("../data/textures/font.png", TextureName::FONT, 0) != 0){
        std::cerr << "font texture not loaded";
        return -1;
    }

    while(!glfwWindowShouldClose(GameView::getWindow())) {
        scene.loop();
    }
    scene.clear();
    return 0;
}