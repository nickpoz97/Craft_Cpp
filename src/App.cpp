#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Interaction/Model.hpp"
#include "stb_image.h"
#include "Interaction/GLError.hpp"

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    GameView game_view{WINDOW_WIDTH, WINDOW_WIDTH, INITIAL_FOV, 0, FULLSCREEN};
    if(!game_view.is_initialized()){
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glLogicOp(GL_INVERT);
    glClearColor(0, 0, 0, 1);

    Shader block_shader{"../data/shaders/block_vertex.glsl", "../data/shaders/block_fragment.glsl"};
    //Shader block_shader{"../data/shaders/testing_cube_v.glsl", "../data/shaders/testing_cube_f.glsl"};
    Shader line_shader{"../data/shaders/line_vertex.glsl", "../data/shaders/line_fragment.glsl"};
    Shader sky_shader{"../data/shaders/sky_vertex.glsl", "../data/shaders/sky_fragment.glsl"};
    Shader text_shader{"../data/shaders/text_vertex.glsl", "../data/shaders/text_fragment.glsl"};

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
    Model model{block_shader, line_shader, sky_shader, text_shader, game_view};

    while(!glfwWindowShouldClose(game_view.get_window())) {
        model.loop();
    }
    return 0;
}