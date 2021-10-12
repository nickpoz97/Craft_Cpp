//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_SCENE_HPP
#define CPP_SCENE_HPP

#include <Rendering/Shader.hpp>
#include "vec2.hpp"
#include "vec3.hpp"
#include "GameView.hpp"
#include "CameraControl.hpp"
#include "Camera.hpp"
#include "Geometry/Chunk.hpp"

enum class ShaderName{
    BLOCK_SHADER,
    LINE_SHADER,
    SKY_SHADER,
    TEXT_SHADER
};

struct ShaderFilesPaths{
    std::string_view vertex_code;
    std::string_view fragment_code;
};

struct GameViewSettings{
    glm::ivec2 windowSize;
    float fov;
};

using ShaderNamesMap =  std::unordered_map<ShaderName, ShaderFilesPaths>;

class Scene {
private:
    static inline std::unique_ptr<Scene> actualInstance{nullptr};
    GameView* gameView;
    CameraControl* cameraControl;
    Camera camera;
    std::unordered_map<ShaderName, Shader> shadersMap{};
    ChunkMap chunkMap{};
    void loadChunkNeighborhood();
    Scene(const GameViewSettings& gvs, const glm::vec3& cameraPos, const glm::vec3 &cameraDirection, const ShaderNamesMap& snm);
public:
    static Scene* setInstance(const GameViewSettings& gvs, const glm::vec3& cameraPos, const glm::vec3& cameraDirection, const ShaderNamesMap& snm);
    void loop();
    static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);
    static void clear();
};


#endif //CPP_SCENE_HPP
