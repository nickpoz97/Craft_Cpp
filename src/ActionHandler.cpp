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
        model_p->set_actual_item(hit_tile);
    }
}

void ActionHandler::on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // no action handler check
    if(!initialized || action != GLFW_PRESS){
        return;
    }

    int control = mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER);
    // indicates if mouse is used for camera motion
    bool exclusive = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

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
        model_p->set_actual_item(TileBlock::items[key - '1']);
    }
    if(key == '0'){
        model_p->set_actual_item(BlockType::GLASS);
    }
    if(key == 'E'){
        model_p->set_next_item();
    }
    if(key == 'R'){
        model_p->set_prev_item();
    }
}

void ActionHandler::initialize(Model* model_address){
    if(!initialized) {
        model_p = model_address;
        player_p = model_p->get_player();
        initialized = (player_p != nullptr) && (model_p != nullptr);
        if(initialized && model_p->get_window()){
            glfwMakeContextCurrent(model_p->get_window());
            glfwSwapInterval(VSYNC);
            glfwSetInputMode(model_p->get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetKeyCallback(model_p->get_window(), on_key);
            glfwSetMouseButtonCallback(model_p->get_window(), on_mouse_button);
            glfwSetScrollCallback(model_p->get_window(), on_scroll);
        }
    }
}

void ActionHandler::on_scroll(GLFWwindow *window, double xdelta, double ydelta) {
    scroll_y_pos += ydelta;

    if(scroll_y_pos < -SCROLL_TRESHOLD){
        model_p->set_prev_item();
        scroll_y_pos = 0;
    }

    if(scroll_y_pos > SCROLL_TRESHOLD){
        model_p->set_next_item();
        scroll_y_pos = 0;
    }
}

void ActionHandler::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    if(!initialized || action != GLFW_PRESS){
        return;
    }

    int control = mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER);
    int exclusive = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if(exclusive){
                control ? on_right_click() : on_left_click();
            }
            else{
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if(exclusive && !control) {on_right_click();}
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            if(exclusive) {on_middle_click();}
        break;
        default:
        break;
    }
}

void ActionHandler::handle_movement(double delta_t) {
    if(!initialized){
        return;
    }
    int x_movement{}, z_movement{};

    model_p->set_ortho(glfwGetKey(model_p->get_window(), 'F') ? 64 : 0);
    model_p->set_fov(glfwGetKey(model_p->get_window(), GLFW_KEY_LEFT_SHIFT) ? 15 : 65);

    glm::vec3 motion_vector = handle_motion_input(delta_t, x_movement, z_movement);

    if(glfwGetKey(model_p->get_window(), GLFW_KEY_SPACE)){
        if(model_p->is_flying()){
            motion_vector.y = 1;
        }
        else if(delta_y == 0){
            delta_y = 8;
        }
    }

    // TODO move everything to player class and check get_highest_block function in model
    float speed = model_p->is_flying() ? 20 : 5;
    int estimate = glm::length(motion_vector * speed + glm::vec3{0, glm::abs(static_cast<float>(delta_y)) * 2, 0});
    int step = glm::max(estimate, 8);

    float ut = delta_t / step;
    motion_vector *= (ut * speed);

    for(int i = 0 ; i < step ; i++){
        delta_y = model_p->is_flying() ? 0 : glm::max(delta_y - ut * 25, -250.0);
        player_p->update_player_position(motion_vector + glm::vec3{0, delta_y * ut, 0});

        auto collision_result = player_p->collide(2);
        player_p->update_player_position(collision_result.second);
        if(collision_result.first){
            delta_y = 0;
        }
    }
    if(player_p->get_position().y < 0){
        player_p->update_player_position(player_p->get_position() + glm::vec3{0, model_p->highest_block(), 0});
    }
}

glm::vec3 ActionHandler::handle_motion_input(double delta_t, int x_movement, int z_movement) {
    if (glfwGetKey(model_p->get_window(), 'W')) { z_movement--; }
    if (glfwGetKey(model_p->get_window(), 'S')) { z_movement++; }
    if (glfwGetKey(model_p->get_window(), 'A')) { x_movement--; }
    if (glfwGetKey(model_p->get_window(), 'D')) { x_movement++; }
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_LEFT)) player_p->increment_player_rotation({-delta_t, 0});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_RIGHT)) player_p->increment_player_rotation({delta_t, 0});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_UP)) player_p->increment_player_rotation({0, delta_t});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_DOWN)) player_p->increment_player_rotation({0, -delta_t});

    glm::vec3 motion_vector{player_p->get_motion_vector(x_movement, z_movement)};
    return motion_vector;
}


/*void ActionHandler::on_light() {
    auto result = player.hit_test(false);
    const auto& hit_pos {result.first};
    const auto& hit_tile_block {result.second};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){

    }
}*/

