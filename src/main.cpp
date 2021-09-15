#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "stb_image.h"

static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

int main() {
    /*if(!glfwInit()){
        std::cerr << "glfw not initialized" << '\n';
        return -1;
    }

    Model model{};
    if(!model.get_window()){
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "glad not initialized" << '\n';
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glLogicOp(GL_INVERT);
    glClearColor(0, 0, 0, 1);

    int load_status = 0;
    if(load_texture("textures/texture.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(load_texture("textures/font.png")){
        std::cerr << "font not loaded";
        return -1;
    }
    if(load_texture("textures/sky.png", GL_CLAMP_TO_EDGE)){
        std::cerr << "sky texture not loaded";
        return -1;
    }

    while(true){
        bool continue_loop = model.loop();
        if(!continue_loop){
            break;
        }
    }

*/
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
    //glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return 0;
}