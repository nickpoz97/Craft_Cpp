//
// Created by ultimatenick on 01/11/21.
//

#include "Interaction/GameView.hpp"
#include "catch2/catch.hpp"
#include "glm/ext/matrix_relational.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

TEST_CASE("Set GameView instance, attributes and destructor") {
  using CraftCpp::GameView;
  int width{1024}, height{768};
  float fov{80.0f};

  {
    std::unique_ptr<GameView> gameView =
        GameView::setInstance(width, height, fov);
    REQUIRE(GameView::isInstantiated());

    // other call should do nothing and return nullptr
    REQUIRE(!GameView::setInstance(width, height, fov));

    REQUIRE((gameView->getWidth() == width && gameView->getHeight() == height &&
             gameView->getFov() == fov));

    REQUIRE(gameView->getScale() == width / height);
    REQUIRE(gameView->getRatio() == static_cast<float>(width) / height);
    REQUIRE(gameView->getOrtho() == 0);

    gameView->setFov(90.0f);
    REQUIRE(gameView->getFov() == 90.0f);

    auto mp = gameView->getProjMatrix(GameView::ProjType::PERSP);
    auto mpCompare = glm::perspective(
        static_cast<float>(
            glm::radians(static_cast<float>(gameView->getFov()))),
        gameView->getRatio(), GameView::z_near, GameView::z_far);
    REQUIRE(glm::all(glm::equal(mp, mpCompare, 0.01f)));
    auto mu = gameView->getProjMatrix(GameView::ProjType::UI);
    auto muCompare = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                                static_cast<float>(height), -1.0f, 1.0f);
    REQUIRE(glm::all(glm::equal(mu, muCompare, 0.01f)));
  }
  REQUIRE(!GameView::isInstantiated());
}
