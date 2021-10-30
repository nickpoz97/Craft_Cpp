#define GLFW_INCLUDE_NONE

#include <iostream>
#include <Interaction/Scene.hpp>

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    CraftCpp::GameViewSettings gvs{{CraftCpp::WINDOW_WIDTH, CraftCpp::WINDOW_WIDTH}, CraftCpp::INITIAL_FOV};
    CraftCpp::ShaderNamesMap snm{};
    snm.emplace(CraftCpp::ShaderName::BLOCK_SHADER, CraftCpp::ShaderFilesPaths{
        "../data/shaders/block_vertex.glsl","../data/shaders/block_fragment.glsl"
    });
    snm.emplace(CraftCpp::ShaderName::TEXT_SHADER, CraftCpp::ShaderFilesPaths{
            "../data/shaders/text_vertex.glsl","../data/shaders/text_fragment.glsl"
    });

    CraftCpp::Scene& scene{*CraftCpp::Scene::setInstance(gvs, {0, 20, 0}, {-90,-10}, snm)};
    if(scene.load_texture("../data/textures/texture.png", CraftCpp::TextureName::GENERAL, 0) != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(scene.load_texture("../data/textures/font.png", CraftCpp::TextureName::FONT, 0) != 0){
        std::cerr << "font texture not loaded";
        return -1;
    }

    while(!glfwWindowShouldClose(CraftCpp::GameView::getWindow())) {
        scene.loop();
    }
    scene.clear();
    return 0;
}