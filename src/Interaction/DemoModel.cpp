//
// Created by ultimatenick on 07/10/21.
//

#include "DemoModel.hpp"

DemoModel::DemoModel(const glm::ivec2& windowSize, float fov, const glm::ivec3 &camerPos,
                     const glm::vec2 &cameraOrientation) :
                     gameView{windowSize.x, windowSize.y, fov},
                     camera({}, {90.0f, 0.0f}),
                     demoInteraction{gameView.getWindow(), camera, {gameView.get_width() / 2, gameView.get_height() / 2}}{

}
