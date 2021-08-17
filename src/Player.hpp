//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>
#include <vec3.hpp>

#include "Cube.hpp"
#include "Model.hpp"

struct Status{
    glm::vec3 position;
    glm::vec2 rotation;
    double t;

    friend Status operator+(const Status& a, const Status& b);
    friend Status operator-(const Status& a, const Status& b);
    friend Status operator*(const Status& a, float p);
};

class Player {
private:
    static std::array<std::array<float,4>,6> ao;
    static std::array<std::array<float,4>,6> light;
    static TileBlock tiles;

    Status actual_status;
    Status former_status1;
    Status former_status2;

    Cube playerCube;
    const Model& model;

    int x_movement = 0;
    int z_movement = 0;

    std::string_view name;
    int id;

    // TODO use Plane class
    static std::array<glm::vec3, 6> get_planes_ortho(const glm::vec3& up, const glm::vec3& right, const glm::vec3& view_dir);
    static std::array<glm::vec3, 6> get_planes_persp(const glm::vec3& up, const glm::vec3& right, const glm::vec3& view_dir, const Frustum& frustum,
                                                     const glm::vec3& view_pos);
public:
    Player(const Model &model, std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation);
    glm::vec3 get_camera_direction_vector();
    glm::vec3 get_motion_vector();
    glm::vec3 get_up_vector();
    glm::vec3 get_right_vector();

    void set_movement(int x, int z);
    void update_player(const glm::vec3& new_position, const glm::vec2& new_rotation={}, bool interpolate={});
    void update_player(const Status& new_status, bool interpolate);
    void interpolate_player();
    void draw();

    std::array<glm::vec3, 6> get_frustum_planes(bool ortho);
};


#endif //CPP_PLAYER_HPP
