//
// Created by ultimatenick on 07/08/21.
//
#define GLFW_INCLUDE_NONE

#include <iostream>
#include "../Geometry/Text2D.hpp"
#include "Model.hpp"
#include "gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"
#include "mat4x4.hpp"
#include "ext/matrix_transform.hpp"

#include "Player.hpp"

Player::Player(std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation)
        :
    actual_status{position, normalize_angles(rotation), glfwGetTime()},
    name{name},
    id{id}
    {
}

glm::vec3 Player::get_motion_vector(int x_movement, int z_movement, bool is_flying, bool jump_action) const{
    const glm::vec2& rotation = actual_status.orientation_degrees;

    if (!z_movement && !x_movement) {
        return {0,0,0};
    }
    float strafe = atan2f(z_movement, x_movement);
    if (is_flying) {
        float m = cosf(rotation.x);
        float y = sinf(rotation.y);
        if (x_movement) {
            if (!z_movement) {
                y = 0;
            }
            m = 1;
        }
        if (z_movement > 0) {
            y = -y;
        }
        if(jump_action){
            y += 1.0f;
        }
        return {cosf(rotation.x + strafe) * m,
            y,
            sinf(rotation.x + strafe) * m
        };
    }
    else {
        return {cosf(rotation.x + strafe),
        0,
        sinf(rotation.x + strafe)};
    }
}

glm::vec3 Player::get_camera_direction_vector() const {
    float yaw = actual_status.orientation_degrees.x;
    float pitch = actual_status.orientation_degrees.y;

    return glm::normalize(glm::vec3{
          cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
          sin(glm::radians(pitch)),
          sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
    });
}

/*void Player::update_player_status(const glm::vec3 &new_position, const glm::vec2 &new_rotation, bool interpolate) {
    if(interpolate){
        former_status1 = former_status2;
        former_status2 = {new_position, new_rotation, glfwGetTime()};

        if (former_status2.orientation_degrees.x - former_status1.orientation_degrees.x > glm::pi<float>()){
            former_status1.orientation_degrees.x += 2 * glm::pi<float>();
        }

        if (former_status1.orientation_degrees.x - former_status2.orientation_degrees.x > glm::pi<float>()){
            former_status1.orientation_degrees.x -= 2 * glm::pi<float>();
        }
    }
    if(!interpolate){
        actual_status = {new_position, new_rotation, glfwGetTime()};
    }
}*/

/*void Player::interpolate_player() {
    double dt1 = former_status2.t - former_status1.t;
    double dt2 = glfwGetTime() - former_status2.t;

    dt1 = glm::min(dt1, 1.0);
    dt1 = glm::max(dt1, 0.1);

    auto p = static_cast<float>(glm::min(dt2/dt1, 1.0));
    update_player(former_status1 + (former_status2 - former_status1) * p, false);
}*/

/*void Player::update_player(const Status& new_status, bool interpolate) {
    update_player_status(new_status.position, new_status.orientation_degrees, interpolate);
}*/

glm::vec3 Player::get_right_vector() const{
    glm::vec3 asy{0.0f, 1.0f, 0.0f};
    return glm::normalize(glm::cross(asy, get_camera_direction_vector()));
}

Block Player::ray_hit(const Chunk& c, bool previous, int max_distance, int step) const {

    const glm::vec3& ray{get_camera_direction_vector()};
    glm::vec3 test_pos{actual_status.position - ray}; // better for loop
    glm::ivec3 block_pos{}; // better for loop
    glm::ivec3 previous_pos{};

    for(int i = 0 ; i < max_distance ; i++){
        test_pos += ray;
        block_pos = glm::round(test_pos);
        if(previous_pos == block_pos){
            continue;
        }
        const TileBlock& material = c.get_block(block_pos);
        if(material.is_obstacle()){
            return (previous) ?
                Block{previous_pos, material} :
                Block{block_pos, material};
        }
        previous_pos = block_pos;
    }

    return Block{};
}

/*HitResult Player::hit_test_face() {
    auto pair{hit_test(false)};
    const TileBlock& w = pair.second;
    const glm::vec3& hit_pos = pair.first;

    if (w.is_obstacle()){
        const glm::vec3 d_hit = hit_test(true).first;
        const glm::vec3 face_normal{d_hit - hit_pos};

        for(auto it{Cube::normals.begin()}; it != Cube::normals.end(); ++it){
            int normal_index = static_cast<int>(it - Cube::normals.begin());
            if(*it == face_normal){
                if(face_normal != glm::vec3{0,1,0}){
                    return {static_cast<Face>(normal_index), hit_pos, true};
                }
                const glm::vec3& actual_pos = actual_status.position;
                int degrees = glm::round(glm::degrees(glm::atan(actual_pos.x - d_hit.x, actual_pos.z - d_hit.z)));
                if (degrees < 0) {
                    degrees += 360;
                }
                int top = ((degrees + 45) / 90) % 4;
                return{static_cast<Face>(4 + top), hit_pos, true};
            }
        }
    }
    return{{},{}, false};
}*/

bool Player::insersects_block(int height, const glm::ivec3& block_pos) const {
    // TODO check if truncation is better than rounding
    const glm::ivec3 int_pos{glm::round(actual_status.position)};

    for(int i = 0 ; i < height ; i++){
        if(int_pos == (block_pos - glm::ivec3{0,i,0})){
            return true;
        }
    }
    return false;
}

const glm::vec3 &Player::get_position() const {
    return actual_status.position;
}

glm::ivec2 Player::get_pq() const {
    return Chunk::chunked(actual_status.position);
}

void Player::update_player_position(const glm::vec3 &new_position) {
    actual_status.position = new_position;
}

void Player::update_player_orientation(const glm::vec2 &new_orientation_deg) {
    actual_status.orientation_degrees = new_orientation_deg;
}

void Player::rotate(const glm::vec2 &angle_degrees) {
    actual_status.orientation_degrees = normalize_angles(actual_status.orientation_degrees + angle_degrees);
}

const glm::vec2 &Player::get_orientation_degrees() const {
    return actual_status.orientation_degrees;
}

void Player::apply_movement(double delta_t, bool is_flying, bool jump_action,
                            const glm::vec2 &horizontal_motion, const std::unordered_map<glm::ivec2, Chunk> &chunk_map) {
    float speed = is_flying ? 20 : 5;

    auto motion_vector = get_motion_vector(horizontal_motion.x, horizontal_motion.y, is_flying, jump_action);
    if(jump_action && !is_flying && delta_y == 0){
        delta_y = 8;
    }

    int estimate = glm::length(motion_vector * speed + glm::vec3{0, glm::abs(static_cast<float>(delta_y)) * 2, 0});
    int step = glm::max(estimate, 8);

    float ut = delta_t / step;
    motion_vector *= (ut * speed);

    for(int i = 0 ; i < step ; i++){
        delta_y = is_flying ? 0 : glm::max(delta_y - ut * 25, -250.0);
        update_player_position(motion_vector + glm::vec3{0, delta_y * ut, 0});

        auto collision_result = collide(2, chunk_map);
        update_player_position(collision_result.second);
        if(collision_result.first){
            delta_y = 0;
        }
    }
    if(actual_status.position.y < 0){
        actual_status.position.y += chunk_map.at(get_pq()).getHighestBlock();
    }
}

Status operator+(const Status &a, const Status &b) {
    return {
        a.position + b.position,
        a.orientation_degrees + b.orientation_degrees,
        0
    };
}

Status operator-(const Status &a, const Status &b) {
    return {
        a.position - b.position,
        a.orientation_degrees - b.orientation_degrees,
        0
    };
}

Status operator*(const Status &a, float p) {
    return {
        p * a.position,
        p * a.orientation_degrees,
        0
    };
}

std::pair<bool, glm::vec3> Player::collide(int height, const std::unordered_map<glm::ivec2, Chunk> &chunk_map) {
    const glm::vec3& position{actual_status.position};
    const Chunk& c{chunk_map.at(get_pq())};
    glm::vec3 collision_point{};
    bool result{};

    const glm::ivec3 int_pos{glm::abs(position)};
    const glm::vec3 decimal_dif_pos(position - static_cast<glm::vec3>(int_pos));
    float pad = 0.25;

    // TODO check (+1 -> value > pad) and (-1 -> value < pad)
    glm::ivec3 signs{glm::step(pad, decimal_dif_pos) - glm::step(pad, -decimal_dif_pos)};
    std::array<glm::vec3, 3> block_pos{{
                                               {int_pos.x + (signs.x) * 1, int_pos.y, int_pos.z},
                                               {int_pos.x, int_pos.y + (signs.y) * 1, int_pos.z},
                                               {int_pos.x, int_pos.y, int_pos.z + (signs.z) * 1},
                                       }};

    for(int y_step = 0; y_step < height ; ++y_step){
        glm::bvec3 enable{
                TileBlock{c.get_block(block_pos[0] + glm::vec3{signs.x, 0, 0} * pad)}.is_obstacle(),
                TileBlock{c.get_block(block_pos[1] + glm::vec3{0, signs.y, 0} * pad)}.is_obstacle(),
                TileBlock{c.get_block(block_pos[2] + glm::vec3{0, 0, signs.z} * pad)}.is_obstacle()
        };

        collision_point.x = (enable.x) ? int_pos.x + signs.x * pad : collision_point.x;
        collision_point.y = (enable.y) ? (result=1, int_pos.x) + signs.x * pad : collision_point.y;
        collision_point.z = (enable.z) ? int_pos.x + signs.x * pad : collision_point.z;
    }
    return {result, collision_point};
}

glm::mat4 Player::get_view_matrix() const {
    return glm::lookAt(get_position(), get_position() + get_camera_direction_vector(), up);
}

glm::vec2 Player::normalize_angles(const glm::vec2 &orientation) {
    float yaw_decimal_part{orientation.x - glm::floor(orientation.x)};
    int yaw_integer_part{static_cast<int>(orientation.x) % static_cast<int>(yaw_limit)};
    float yaw = yaw_integer_part + yaw_decimal_part;
    float pitch = glm::clamp(orientation.y, -pitch_limit, pitch_limit);
    return {yaw, pitch};
}
