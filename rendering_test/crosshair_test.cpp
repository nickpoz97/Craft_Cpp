//
// Created by ultimatenick on 18/09/21.
//

#include <vector>
#include <iostream>
#include "../src/Interaction/GameView.hpp"
#include "../src/Rendering/Shader.hpp"
#include "../src/Geometry/Vertex.hpp"
#include "../src/Geometry/Crosshair.hpp"
#include "../src/Rendering/OpenglBuffer.hpp"
#include "vec4.hpp"
#include "gtc/matrix_transform.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
    GameView game_view{800, 600, 45, 0, false};
    if(!game_view.is_initialized()){
        return -1;
    }
    glfwSetFramebufferSizeCallback(game_view.get_window(), framebuffer_size_callback);

    Shader s{"../data/shaders/line_vertex.glsl", "../data/shaders/line_fragment.glsl"};
    /*std::vector<Standard2DVertex> vertices{{
            {{-0.5f, -0.5f}}, // left
            {{0.5f, -0.5f}}, // right
            {{0.0f, 0.5f}}  // top
    }};*/
    /*std::vector<Standard2DVertex> vertices{{
            {{-0.5f, 0.0f}}, // left
            {{0.5f, 0.0f}}, // right
            {{0.0f, 0.5f}},  // top
            {{0.0f, -0.5f}}  // bottom
    }};*/
    //Shader s{"../data/shaders/line_vertex.glsl", "../data/shaders/line_fragment.glsl"};

    /*for(int i = 0 ; i < 9 ; i++){
        std::cout << *(reinterpret_cast<float*>(vertices.data()) + i) << '\n';
    }
    const std::vector<Standard3DVertex>& ref{vertices};

    std::cout << ref.size() * sizeof(Standard3DVertex) << '\n';
    */

    //RenderableEntity<Standard2DVertex> crosshair{vertices};

    /*OpenglBuffer<Standard2DVertex> openglBuffer{};
    openglBuffer.store_data(vertices);*/

    s.use();

    glm::mat4 proj = glm::ortho(0.0, 800.0, 0.0, 600.0, -10.0, 10.0);
    glm::vec4 pos =  proj * glm::vec4(600.0, 600.0, 4.0, 1.0);
    std::cout << "(" << pos.x << ',' << pos.y << ',' << pos.z << ',' << pos.w << ")\n";

    while(!glfwWindowShouldClose(game_view.get_window())){
        processInput(game_view.get_window());
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s.set_viewproj(game_view.get_proj_matrix(GameView::ProjType::UI));

        Crosshair crossHair{game_view.get_width(), game_view.get_height(), game_view.get_scale()};
        crossHair.render_lines();

        glfwSwapBuffers(game_view.get_window());
        glfwPollEvents();
    }

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}