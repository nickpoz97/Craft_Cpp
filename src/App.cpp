#define GLFW_INCLUDE_NONE

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Interaction/Model.hpp"
#include "stb_image.h"
#include "Rendering/GLError.hpp"

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    GameViewSettings gvs{WINDOW_WIDTH, WINDOW_WIDTH, INITIAL_FOV, 0, FULLSCREEN};
    ShaderNamesMap snm{};
    snm.emplace(ShaderName::BLOCK_SHADER, ShaderFilesPaths{
        "../data/shaders/block_vertex.glsl","../data/shaders/block_fragment.glsl"
    });
    snm.emplace(ShaderName::LINE_SHADER, ShaderFilesPaths{
            "../data/shaders/line_vertex.glsl", "../data/shaders/line_fragment.glsl"
    });
    snm.emplace(ShaderName::SKY_SHADER, ShaderFilesPaths{
            "../data/shaders/sky_vertex.glsl", "../data/shaders/sky_fragment.glsl"
    });
    snm.emplace(ShaderName::TEXT_SHADER, ShaderFilesPaths{
            "../data/shaders/text_vertex.glsl", "../data/shaders/text_fragment.glsl"
    });

    Model model{snm, gvs};
    if(Model::load_texture("../data/textures/texture.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(Model::load_texture("../data/textures/font.png")){
        std::cerr << "font not loaded";
        return -1;
    }
    if(Model::load_texture("../data/textures/sky.png", GL_CLAMP_TO_EDGE)){
        std::cerr << "sky texture not loaded";
        return -1;
    }

    while(!glfwWindowShouldClose(model.get_window())) {
        //model.loop();
        model.loop();
    }
    return 0;
}