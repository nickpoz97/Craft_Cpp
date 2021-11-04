//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_SCENE_HPP
#define CPP_SCENE_HPP

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Rendering/Shader.hpp"
#include "Interaction/GameView.hpp"
#include "Interaction/CameraControl.hpp"
#include "Interaction/Camera.hpp"
#include "Geometry/Chunk.hpp"

namespace CraftCpp {
enum class ShaderName {
    BLOCK_SHADER,
    TEXT_SHADER
};

enum class TextureName {
    FONT,
    GENERAL
};

struct ShaderFilesPaths {
    std::string_view vertexCode;
    std::string_view fragmentCode;
};

struct GameViewSettings {
    glm::ivec2 windowSize;
    float fov;
};

using ShaderNamesMap = std::unordered_map<ShaderName, ShaderFilesPaths>;

class Scene {
private:
    static inline Scene* actualInstance{nullptr};
    std::unique_ptr<GameView> gameView;
    std::unique_ptr<CameraControl> cameraControl;
    Camera camera;
    std::unordered_map<ShaderName, Shader> shaders{};
    std::array<uint, 2> textureSamplers;
    ChunkMap chunkMap{};

    void render_text(int justify, const glm::vec2 &position, float n, std::string_view text) const;

    void showInfoText() const;

    void loadChunkNeighborhood();

    void deleteDistantChunks();

    void waitThreads() const;

    Scene(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec2 &cameraRotation,
          const ShaderNamesMap &snm);

public:
    static std::unique_ptr<Scene>
    setInstance(const GameViewSettings &gvs, const glm::vec3 &cameraPos, const glm::vec2 &cameraRotation,
                const ShaderNamesMap &snm);

    void loop();

    int load_texture(std::string_view path, TextureName textureName, GLint clampType = GL_REPEAT);
    ~Scene();

    void loadAndRenderChunks();
};
}

#endif //CPP_SCENE_HPP
