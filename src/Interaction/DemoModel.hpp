//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_DEMOMODEL_HPP
#define CPP_DEMOMODEL_HPP

#include "vec2.hpp"
#include "vec3.hpp"
#include "GameView.hpp"
#include "DemoInteraction.hpp"
#include "FlyingCamera.hpp"
#include "Geometry/Chunk.hpp"

class DemoModel {
private:
    GameView gameView;
    DemoInteraction demoInteraction;
    FlyingCamera camera;

    ChunkMap chunks{};
public:
    DemoModel(const glm::ivec2& windowSize, float fov, const glm::ivec3& camerPos, const glm::vec2& cameraOrientation);
};


#endif //CPP_DEMOMODEL_HPP
