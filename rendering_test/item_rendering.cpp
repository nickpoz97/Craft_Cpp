//
// Created by ultimatenick on 20/09/21.
//

#include <iostream>
#include "../src/Interaction/GameView.hpp"
#include "glad/glad.h"
#include "../src/Rendering/Shader.hpp"
#include "gtc/matrix_transform.hpp"
#include "../src/Geometry/Item.hpp"
#include "../src/Geometry/CubicObject.hpp"
#include "stb_image.h"
#include "../src/Interaction/Model.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
    GameView game_view{800, 600, 45, 0, false};
    if(!game_view.is_initialized()){
        return -1;
    }
    glfwSetFramebufferSizeCallback(game_view.get_window(), framebuffer_size_callback);

    Shader s{"../data/shaders/block_vertex.glsl", "../data/shaders/block_fragment.glsl"};
    s.use();
    s.set_viewproj(glm::mat4{1.0f});

    if(Model::load_texture("../data/textures/texture.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }
    if(Model::load_texture("../data/textures/sky.png") != 0){
        std::cerr << "general texture not loaded";
        return -1;
    }

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    s.set_sampler(0);

    float size = 64 * game_view.get_scale();
    /*auto get_offset = [size](float axis){return 1 - size / axis * 2;};

    glm::vec2 offset{get_offset(game_view.get_width()),get_offset(game_view.get_height())};*/

    Item c{
            BlockType::SAND, {}
    };

    while(!glfwWindowShouldClose(game_view.get_window())){
        processInput(game_view.get_window());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //c.print_vertex_info();
        c.render_object();
        //ob.draw_triangles();

        glfwSwapBuffers(game_view.get_window());
        glfwPollEvents();
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
