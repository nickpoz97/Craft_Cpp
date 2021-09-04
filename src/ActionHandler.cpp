//
// Created by ultimatenick on 04/09/21.
//

#include "ActionHandler.hpp"

void ActionHandler::on_left_click() {
    if(!initialized){
        return;
    }
    Block result = player_p->hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        model_p->set_block(hit_pos, BlockType::EMPTY);
        model_p->record_block(hit_pos, BlockType::EMPTY);
        if(model_p->get_block(hit_pos + glm::ivec3{0,1,0}));{
            model_p->set_block(hit_pos + glm::ivec3{0,1,0}, BlockType::EMPTY);
        }
    }
}

void ActionHandler::on_right_click() {
    if(!initialized){
        return;
    }
    Block result = player_p->hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        if(!player_p->insersects_block(2, hit_pos)){
            // using actual item index
            model_p->set_block(hit_pos);
            model_p->record_block(hit_pos);
        }
    }
}

void ActionHandler::on_middle_click() {
    if(!initialized){
        return;
    }
    const Block hit_block{player_p->hit_test(false)};
    const TileBlock& hit_tile {hit_block.second};
    if(hit_tile.is_user_buildable()){
        model_p->set_item_index(hit_tile);
    }
}

void ActionHandler::on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // no action handler check
    if(!initialized){
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
            on_right_click();
        }
        else{
            on_left_click();
        }
    }
    if (key == GLFW_KEY_TAB) {
        model_p->switch_flying();
    }
    if (key >= '1' && key <= '9') {
        // +1 to skip empty
        model_p->set_item_index(TileBlock::items[key - '1']);
    }
    if(key == '0'){
        model_p->set_item_index(BlockType::GLASS);
    }
    if(key == 'E'){
        model_p->set_item_index(TileBlock::items[(model_p->get_item_index() + 1) % TileBlock::items.size()]);
    }
    if(key == 'R'){
        int index = model_p->get_item_index() - 1;
        index = (index < 0) ? static_cast<int>(TileBlock::items.size()) - 1 : index;
        model_p->set_item_index(TileBlock::items[index]);
    }
}

void ActionHandler::attach_components(Player* player_address, Model* model_address){
    if(!initialized) {
        player_p = player_address;
        model_p = model_address;
        initialized = (player_p != nullptr) && (model_p != nullptr);
    }
}

/*void ActionHandler::on_light() {
    auto result = player.hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){

    }
}*/

