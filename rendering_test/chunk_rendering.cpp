//
// Created by ultimatenick on 21/09/21.
//

#include <iostream>
#include <fmt/ostream.h>
#include "../src/Geometry/Chunk.hpp"
#include "../src/Interaction/GameView.hpp"
#include "../src/Rendering/Shader.hpp"
#include "gtc/matrix_transform.hpp"
#include "../src/Interaction/Model.hpp"
#include "../src/Geometry/Item.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

glm::vec3 cameraPos   = glm::vec3(0.0f, 20.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, .0f) - cameraPos;
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

float deltaTime{};
float lastFrame = 0.0f;

class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using clock_type = std::chrono::steady_clock;
    using second_type = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_type> m_beg;

public:
    Timer() : m_beg { clock_type::now() }
    {
    }

    void reset()
    {
        m_beg = clock_type::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
    }
};



int main() {
    GameView game_view{800, 600, 45, 0, false};

    if (!game_view.is_initialized()) {
        return -1;
    }
    glfwSetFramebufferSizeCallback(game_view.get_window(), framebuffer_size_callback);
    glfwSetCursorPosCallback(game_view.get_window(), mouse_callback);
    glfwSetInputMode(game_view.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader s{"../data/shaders/block_vertex.glsl", "../data/shaders/block_fragment.glsl"};
    s.use();

    if (Model::load_texture("../data/textures/texture.png") != 0) {
        std::cerr << "general texture not loaded";
        return -1;
    }

    s.set_sampler(0);

    glm::mat4 proj{game_view.get_proj_matrix(GameView::ProjType::PERSP)};

    Timer t;
    ChunkMap chunks{};
    for(int dp = -5 ; dp < 5 ; dp++){
        for(int dq = -5 ; dq < 5 ; dq++){
            chunks.emplace(glm::ivec2{dp, dq}, Chunk{{dp, dq}, false, chunks});
        }
    }
    for(auto& pair : chunks){
        pair.second.init_chunk();
    }
    Chunk::wait_threads();

    std::list<std::thread> rendering_threads{};
    for(auto& pair : chunks){
        pair.second.update_buffer();
    }
    std::cout << "Time elapsed: " << t.elapsed() << " seconds\n";


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

#ifdef DEBUG
    std::cout << "debug build\n";
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(game_view.get_window())) {
        processInput(game_view.get_window());

        processInput(game_view.get_window());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        s.set_viewproj(proj * view);
        for (const auto & pair: chunks) {
            const Chunk& c = pair.second;
            if(c.is_visible(proj * view)) {
                //fmt::print("pq: {}{}, min_x: {}, max_x: {}, min_z: {}, max_z: {}\n", c.pq.x, c.pq.y ,c.get_min_x(), c.get_max_x(), c.get_min_z(), c.get_max_x());
                c.render_object();
            }
        }
        //chunks.front().render_object({});
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

    float cameraSpeed = 0.05 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
