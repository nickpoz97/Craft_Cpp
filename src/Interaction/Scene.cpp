//
// Created by ultimatenick on 07/10/21.
//

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <Geometry/Text2D.hpp>
#include <fmt/format.h>
#include "Interaction/Scene.hpp"
#include "stbi/stb_image.h"

namespace CraftCpp {
Scene::Scene(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec2 &cameraRotation,
             const ShaderNamesMap &snm) :
        gameView{GameView::setInstance(gvs.windowSize.x, gvs.windowSize.y, gvs.fov)},
        camera{cameraPos, cameraRotation},
        cameraControl{CameraControl::setInstance(camera)} {

    glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    for (auto &pair: snm) {
        shaders.emplace(pair.first, Shader{pair.second.vertex_code, pair.second.fragment_code});
    }

    loadChunkNeighborhood();
}

void Scene::loadChunkNeighborhood() {
    for (int dp = -CREATE_CHUNK_RADIUS; dp <= CREATE_CHUNK_RADIUS; dp++) {
        for (int dq = -CREATE_CHUNK_RADIUS; dq <= CREATE_CHUNK_RADIUS; dq++) {
            const glm::ivec2 chunkPq{camera.getPq() + glm::ivec2{dp, dq}};
            chunkMap.try_emplace(chunkPq, Chunk{chunkPq});
        }
    }

    for (auto &pair: chunkMap) {
        Chunk &c = pair.second;
        if (c.needInitCall()) {
            c.init_chunk();
        }
    }
}

void Scene::deleteDistantChunks() {
    auto it = chunkMap.begin();
    while (it != chunkMap.end()) {
        Chunk &c{it->second};
        if (get_chunk_distance(c.pq, camera.getPq()) >= DELETE_CHUNK_RADIUS) {
            c.wait_thread();
            auto eraseIt = it;
            ++it;
            chunkMap.erase(eraseIt);
        } else {
            ++it;
        }
    }
}

void Scene::loop() {
    cameraControl->processKeyboardInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const Shader &s{shaders.at(ShaderName::BLOCK_SHADER)};
    glm::mat4 viewProj{gameView->get_proj_matrix(GameView::PERSP) * camera.getViewMatrix()};
    s.use();
    s.set_viewproj(viewProj);
    s.set_sampler(textureSamplers[static_cast<int>(TextureName::GENERAL)]);
    s.set_camera(camera.getPos());
    s.set_extra_uniform("fog_distance", static_cast<float>(RENDER_CHUNK_RADIUS * CHUNK_SIZE));

    s.set_extra_uniform("sky_color", glm::vec3{0.2, 0.2, 0.5});

    loadChunkNeighborhood();
    for (const auto &pair: chunkMap) {
        const Chunk &c = pair.second;
        if (pair.second.is_visible(viewProj)) {
            c.render_object();
        }
    }
    showInfoText();
    glfwSwapBuffers(GameView::getWindow());
    glfwPollEvents();
    deleteDistantChunks();
}

std::unique_ptr<Scene>
Scene::setInstance(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec2 &cameraRotation,
                   const ShaderNamesMap &snm) {
    if (!actualInstance) {
        actualInstance = new Scene{gvs, cameraPos, cameraRotation, snm};
    }
    return std::unique_ptr<Scene>{actualInstance};
}

int Scene::load_texture(std::string_view path, TextureName textureName, GLint clamp_type) {
    uint texture_id;
    glGenTextures(1, &texture_id);

    static uint texture_index = 0;
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_type);

    stbi_set_flip_vertically_on_load(true); // st coords instead of uv coords
    int width, height, nr_channels;
    unsigned char *data = stbi_load(path.data(), &width, &height, &nr_channels, 0);

    if (!data) {
        stbi_image_free(data);
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    textureSamplers[static_cast<int>(textureName)] = texture_index++;
    return 0;
}

Scene::~Scene() {
    waitThreads();
    actualInstance = nullptr;
}

void Scene::waitThreads() const {
    for (const auto &pair: chunkMap) {
        pair.second.wait_thread();
    }
}

void Scene::render_text(int justify, const glm::vec2 &position, float n, std::string_view text) const {
    const Shader &shader = shaders.at(ShaderName::TEXT_SHADER);

    shader.use();
    shader.set_viewproj(gameView->get_proj_matrix(GameView::ProjType::UI));
    shader.set_sampler(textureSamplers[static_cast<int>(TextureName::FONT)]);
    const glm::vec2 justified_position{position - glm::vec2{n * justify * (text.size() - 1) / 2, 0}};
    Text2D text2d{justified_position, n, text};
    text2d.render_object();
}

void Scene::showInfoText() const {
    float ts = 12 * gameView->get_scale();
    float tx = ts / 2;
    float ty = gameView->get_height() - ts;

    const glm::ivec2 &pq{camera.getPq()};
    const glm::vec3 &pos{camera.getPos()};
    const glm::vec3 &front{camera.getFrontVector()};

    std::string s{fmt::format("nChunks: {} ,camPq: ({},{}) ,camPos: ({:.2f},{:.2f},{:.2f}), "
                              "front: ({:.2f},{:.2f},{:.2f})\n",
                              chunkMap.size(), pq.x, pq.y, pos.x, pos.y, pos.z, front.x, front.y, front.z)};

    render_text(ALIGN_LEFT, {tx, ty}, ts, s);
}
}