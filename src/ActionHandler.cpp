//
// Created by ultimatenick on 04/09/21.
//

#include "ActionHandler.hpp"

ActionHandler::ActionHandler(const Player &player, Model &model) :
    player{player},
    model{model}{
    instance.reset(this);
}

void ActionHandler::on_left_click() const{
    Block result = player.hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        model.set_block(hit_pos, BlockType::EMPTY);
        model.record_block(hit_pos, BlockType::EMPTY);
        if(model.get_block(hit_pos + glm::ivec3{0,1,0}));{
            model.set_block(hit_pos + glm::ivec3{0,1,0}, BlockType::EMPTY);
        }
    }
}

void ActionHandler::on_right_click() const{
    Block result = player.hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        if(!player.insersects_block(2, hit_pos)){
            // using actual item index
            model.set_block(hit_pos);
            model.record_block(hit_pos);
        }
    }
}

void ActionHandler::on_middle_click() const {
    const Block hit_block{player.hit_test(false)};
    const TileBlock& hit_tile {hit_block.second};
    if(hit_tile.is_user_buildable()){
        model.set_item_index(hit_tile);
    }
}

void ActionHandler::on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // no action handler check
    if(!get_instance()){
        return;
    }

    int control = mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER);
    // indicates if mouse is used for camera motion
    bool exclusive = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
    if (action != GLFW_PRESS) {
        return;
    }
    // esc -> mouse arrow
    if(key == GLFW_KEY_ESCAPE && exclusive){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if(key == GLFW_KEY_ENTER){
        if(control){
            get_instance()->on_right_click();
        }
        else{
            get_instance()->on_left_click();
        }
    }
}

std::shared_ptr<const ActionHandler> ActionHandler::get_instance() {
    return instance;
}

/*void ActionHandler::on_light() {
    auto result = player.hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){

    }
}*/

