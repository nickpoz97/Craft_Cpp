//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>

#include "vec3.hpp"
#include "Chunk.hpp"

struct Status{
    glm::vec3 position;
    glm::vec2 rotation;
    double t;

    friend Status operator+(const Status& a, const Status& b);
    friend Status operator-(const Status& a, const Status& b);
    friend Status operator*(const Status& a, float p);
};

/*struct HitResult{
    Face face;
    glm::vec3 position;
    bool is_happened;
};*/

class Player {
private:
    Status actual_status;
    Status former_status1;
    Status former_status2;

    std::string_view name;

    int id;

    //Frustum frustum;

public:
    const Status &getActualStatus() const;
    const glm::vec3& get_position() const;
    const glm::vec2& get_rotation() const;
    //const Frustum &getFrustum() const;


    Player(std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation);
    glm::vec3 get_camera_direction_vector() const;
    glm::vec3 get_motion_vector(int x_movement, int z_movement, bool is_flying) const;
    glm::vec3 get_up_vector() const;
    glm::vec3 get_right_vector() const;

    void update_player_status(const glm::vec3& new_position, const glm::vec2& new_rotation, bool interpolate);
    void update_player_position(const glm::vec3& new_position);
    void update_player_rotation(const glm::vec2& new_rotation);
    void increment_player_rotation(const glm::ivec2& increment);

    void update_player(const Status& new_status, bool interpolate);
    void interpolate_player();
    Block ray_hit(const Chunk& c, bool previous, int max_distance, int step = 32) const;
    //HitResult hit_test_face();
    bool insersects_block(int height, const glm::ivec3& block_pos) const;
    glm::ivec2 get_pq() const;
};


#endif //CPP_PLAYER_HPP
