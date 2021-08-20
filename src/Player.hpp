//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>
#include <vec3.hpp>

#include "Cube.hpp"
#include "Model.hpp"
#include "frustum.hpp"

struct Status{
    glm::vec3 position;
    glm::vec2 rotation;
    double t;

    friend Status operator+(const Status& a, const Status& b);
    friend Status operator-(const Status& a, const Status& b);
    friend Status operator*(const Status& a, float p);
};

struct HitResult{
    Face face;
    glm::vec3 position;
    bool is_happened;
};

class Player {
private:
    static std::array<std::array<float,4>,6> ao;
    static std::array<std::array<float,4>,6> light;
    static TileBlock tiles;

    Status actual_status;

private:
    Status former_status1;
    Status former_status2;

    Cube playerCube;
    const Model& model;

    int x_movement = 0;
    int z_movement = 0;

    std::string_view name;
    int id;

    Frustum frustum;
public:
    const Status &getActualStatus() const;
    const Frustum &getFrustum() const;

    Player(const Model &model, std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation);
    glm::vec3 get_camera_direction_vector() const;
    glm::vec3 get_motion_vector() const;
    glm::vec3 get_up_vector() const;
    glm::vec3 get_right_vector() const;

    void set_movement(int x, int z);
    void update_player(const glm::vec3& new_position, const glm::vec2& new_rotation={}, bool interpolate={});
    void update_player(const Status& new_status, bool interpolate);
    void interpolate_player();
    void draw();
    std::pair<glm::vec3, Item> hit_test(bool previous);
    std::std::pair<glm::vec3, Item> ray_hit(const Chunk& c, bool previous, int max_distance, int step = 32);
    HitResult hit_test_face();
    bool collide(int height);
};


#endif //CPP_PLAYER_HPP
