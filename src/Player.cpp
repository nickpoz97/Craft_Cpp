//
// Created by ultimatenick on 07/08/21.
//
#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include <cmath>
#include "mat4x4.hpp"
#include "ext/matrix_transform.hpp"

#include "Player.hpp"
//#include "frustum.hpp"
#include "Wrapper.hpp"
#include "Chunk.hpp"

Player::Player(std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation)
        :
    actual_status{position, rotation, glfwGetTime()},
    former_status1{actual_status},
    former_status2{actual_status},
    name{name},
    id{id}
    /*,
    frustum{
        model.getFov(),
        model.z_near,
        model.z_far,
        model.getWidth() / model.getHeight(),
        *this}*/
    {

    glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position);
    transform = glm::rotate(transform, rotation.y, {0, 1, 0});
    transform = glm::rotate(transform, rotation.x, {1, 0, 0});

    //frustum.update(false);
}

glm::vec3 Player::get_motion_vector(int x_movement, int z_movement, bool is_flying) const{
    const glm::vec2& rotation = actual_status.rotation;

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
    const glm::vec2& rotation = actual_status.rotation;

    float m = cosf(rotation.y);
    return glm::normalize(glm::vec3{
        cosf(rotation.x - glm::radians(90.0f)) * m,
        sinf(rotation.y),
        sinf(rotation.x - glm::radians(90.0f)) * m
    });
}

void Player::update_player_status(const glm::vec3 &new_position, const glm::vec2 &new_rotation, bool interpolate) {
    if(interpolate){
        former_status1 = former_status2;
        former_status2 = {new_position, new_rotation, glfwGetTime()};

        if (former_status2.rotation.x - former_status1.rotation.x > glm::pi<float>()){
            former_status1.rotation.x += 2* glm::pi<float>();
        }

        if (former_status1.rotation.x - former_status2.rotation.x > glm::pi<float>()){
            former_status1.rotation.x -= 2*glm::pi<float>();
        }
    }
    else{
        actual_status = {new_position, new_rotation, glfwGetTime()};
    }
}

void Player::interpolate_player() {
    double dt1 = former_status2.t - former_status1.t;
    double dt2 = glfwGetTime() - former_status2.t;

    dt1 = glm::min(dt1, 1.0);
    dt1 = glm::max(dt1, 0.1);

    auto p = static_cast<float>(glm::min(dt2/dt1, 1.0));
    update_player(former_status1 + (former_status2 - former_status1) * p, false);
}

void Player::update_player(const Status& new_status, bool interpolate) {
    update_player_status(new_status.position, new_status.rotation, interpolate);
}

glm::vec3 Player::get_right_vector() const{
    glm::vec3 asy{0.0f, 1.0f, 0.0f};
    return glm::normalize(glm::cross(asy, get_camera_direction_vector()));
}

glm::vec3 Player::get_up_vector() const{
    return glm::cross(get_camera_direction_vector(), get_right_vector());
}

const Status &Player::getActualStatus() const {
    return actual_status;
}

/*const Frustum &Player::getFrustum() const {
    return frustum;
}*/

Block Player::ray_hit(const Chunk& c, bool previous, int max_distance, int step) const {

    const glm::vec3& ray{get_camera_direction_vector()};
    glm::vec3 test_pos{actual_status.position - ray}; // better for loop
    glm::ivec3 test_pos_rounded{}; // better for loop
    glm::ivec3 previous_pos{};

    for(int i = 0 ; i < max_distance ; i++){
        test_pos += ray;
        // TODO check if truncation is better than rounding
        test_pos_rounded = glm::round(test_pos);
        if(previous_pos == test_pos_rounded){
            continue;
        }
        const TileBlock& material = c.get_block(test_pos_rounded);
        if(material.is_obstacle()){
            return (previous) ?
                Block{previous_pos, material} :
                Block{test_pos_rounded, material};
        }
        previous_pos = test_pos_rounded;
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

void Player::update_player_rotation(const glm::vec2 &new_rotation) {
    actual_status.rotation = new_rotation;
}

void Player::increment_player_rotation(const glm::ivec2 &increment) {
    actual_status.rotation += increment;
}

const glm::vec2 &Player::get_rotation() const {
    return actual_status.rotation;
}

Status operator+(const Status &a, const Status &b) {
    return {
        a.position + b.position,
        a.rotation + b.rotation,
        0
    };
}

Status operator-(const Status &a, const Status &b) {
    return {
        a.position - b.position,
        a.rotation - b.rotation,
        0
    };
}

Status operator*(const Status &a, float p) {
    return {
        p * a.position,
        p * a.rotation,
        0
    };
}

