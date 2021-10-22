//
// Created by ultimatenick on 07/10/21.
//

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Scene.hpp"
#include "stb_image.h"

Scene::Scene(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec3 &cameraDirection,
             const ShaderNamesMap& snm) :
        gameView{GameView::setInstance(gvs.windowSize.x, gvs.windowSize.y, gvs.fov)},
        camera{cameraPos, cameraDirection},
        cameraControl{CameraControl::setInstance(camera)}{
    glfwSetInputMode(gameView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
    glfwSetInputMode(gameView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif

    glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    auto& shaderPath{snm.at(ShaderName::BLOCK_SHADER)};

    shadersMap.emplace(ShaderName::BLOCK_SHADER, Shader{shaderPath.vertex_code, shaderPath.fragment_code});

    loadChunkNeighborhood();
}

void Scene::loadChunkNeighborhood(){
    for(int dp = -CREATE_CHUNK_RADIUS ; dp <= CREATE_CHUNK_RADIUS ; dp++){
        for(int dq = -CREATE_CHUNK_RADIUS ; dq <= CREATE_CHUNK_RADIUS ; dq++){
            const glm::ivec2 chunkPq{camera.getPq() + glm::ivec2{dp, dq}};
            auto result = chunkMap.try_emplace(chunkPq, Chunk{chunkPq});
            bool isInserted{result.second};
            auto record{result.first};
            if(isInserted) {
                (record->second).init_chunk();
            }
        }
    }
}

void Scene::deleteDistantChunks(){
    auto it = chunkMap.begin();
    while(it != chunkMap.end()){
        Chunk& c{it->second};
        if(get_chunk_distance(c, camera.getPq()) >= DELETE_CHUNK_RADIUS){
            c.wait_thread();
            auto eraseIt = it;
            ++it;
            chunkMap.erase(eraseIt);
        }
        else{
            ++it;
        }
    }
}

void Scene::loop() {
    cameraControl->processKeyboardInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const Shader& s{shadersMap.at(ShaderName::BLOCK_SHADER)};
    glm::mat4 viewProj{gameView->get_proj_matrix(GameView::PERSP) * camera.getViewMatrix()};
    s.use();
    s.set_viewproj(viewProj);
    s.set_sampler(0);
    s.set_camera(camera.getPos());
    s.set_extra_uniform("fog_distance", static_cast<float>(RENDER_CHUNK_RADIUS * CHUNK_SIZE));

    s.set_extra_uniform("sky_color", glm::vec3{0.2, 0.2, 0.5});

    loadChunkNeighborhood();
    for(const auto& pair : chunkMap){
        const Chunk& c = pair.second;
        if(pair.second.is_visible(viewProj)){
            c.render_object();
        }
    }
    glfwSwapBuffers(GameView::getWindow());
    glfwPollEvents();
    deleteDistantChunks();
}

Scene *
Scene::setInstance(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec3 &cameraDirection,
                   const ShaderNamesMap &snm) {
    if(!actualInstance){
        actualInstance.reset(new Scene{gvs, cameraPos, cameraDirection, snm});
    }
    return actualInstance.get();
}

int Scene::load_texture(std::string_view path, GLint clamp_type) {
    unsigned texture_id;
    glGenTextures(1, &texture_id);

    static unsigned texture_index = 0;
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_type);

    stbi_set_flip_vertically_on_load(true); // st coords instead of uv coords
    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.data(), &width, &height, &nr_channels, 0);

    if(!data){
        stbi_image_free(data);
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    ++texture_index;
    return 0;
}

void Scene::clear() {
    waitThreads();
    actualInstance.reset(nullptr);
    GameView::clear();
}

void Scene::waitThreads() const{
    for(const auto& pair : chunkMap){
        pair.second.wait_thread();
    }
}
