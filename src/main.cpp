#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "stb_image.h"
#include "GLError.hpp"

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    GameView game_view{WINDOW_WIDTH, WINDOW_WIDTH, INITIAL_FOV, 0, FULLSCREEN};
    if(!game_view.is_initialized()){
        return -1;
    }

    Shader block_shader{"../data/shaders/block_vertex.glsl", "../data/shaders/block_fragment.glsl"};
    Shader line_shader{"../data/shaders/line_vertex.glsl", "../data/shaders/line_fragment.glsl"};
    Shader sky_shader{"../data/shaders/sky_vertex.glsl", "../data/shaders/sky_fragment.glsl"};
    Shader text_shader{"../data/shaders/text_vertex.glsl", "../data/shaders/text_fragment.glsl"};

    Model model{block_shader, line_shader, sky_shader, text_shader, game_view};
    if(!model.get_window()){
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glLogicOp(GL_INVERT);
    glClearColor(0, 0, 0, 1);

    if(load_texture("../data/textures/texture.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(load_texture("../data/textures/font.png")){
        std::cerr << "font not loaded";
        return -1;
    }
    if(load_texture("../data/textures/sky.png", GL_CLAMP_TO_EDGE)){
        std::cerr << "sky texture not loaded";
        return -1;
    }

    while(!glfwWindowShouldClose(game_view.get_window())) {
        model.loop();
    }
    return 0;
}

int load_texture(std::string_view path, GLint clamp_type){
    static unsigned texture_index = 0;
    glActiveTexture(GL_TEXTURE0 + texture_index);
    ++texture_index;

    unsigned texture;
    glGenTextures(1, &texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_type);

    stbi_set_flip_vertically_on_load(true); // st coords instead of uv coords
    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.data(), &width, &height, &nr_channels, 0);

    if(!data){
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return 0;
}