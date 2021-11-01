//
// Created by ultimatenick on 01/11/21.
//

#include "Interaction/GameView.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Set the unique GameView instance and check destructor"){
    using CraftCpp::GameView;
    int width{1024}, height{768};
    float fov{80.0f};

    {
        std::unique_ptr<GameView> gameView = GameView::setInstance(width, height, fov);
        REQUIRE(GameView::isInstantiated());
        REQUIRE((gameView->get_width() == width && gameView->get_height() == height && gameView->get_fov() == fov));
    }
    REQUIRE(!GameView::isInstantiated());
}
