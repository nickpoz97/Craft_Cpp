//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>

#include "vec3.hpp"
#include "../Geometry/Chunk.hpp"

struct Status{
    glm::vec3 position;
    glm::vec2 orientation_degrees;
    double t;

    /*friend Status operator+(const Status& a, const Status& b);
    friend Status operator-(const Status& a, const Status& b);
    friend Status operator*(const Status& a, float p);*/
};

/*struct HitResult{
    Face face;
    glm::vec3 position;
    bool is_happened;
};*/

class Player {
private:
    Status actual_status;
    //Status former_status1;
    //Status former_status2;
    static glm::vec2 normalize_angles(const glm::vec2& orientation);

    double delta_y{};

    std::string_view name;

    int id;

    //Frustum frustum;

public:
    static constexpr float yaw_limit = 360.0f;
    static constexpr float pitch_limit = 89.0f;
    static constexpr glm::vec3 up{0.0, 1.0, 0.0};

    Player(std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation);

    const glm::vec3& get_position() const;
    const glm::vec2& get_orientation_degrees() const;
    glm::vec3 get_camera_direction_vector() const;
    glm::vec3 get_motion_vector(int x_movement, int z_movement, bool is_flying, bool jump_action) const;
    glm::vec3 get_right_vector() const;
    glm::mat4 get_view_matrix() const;

    //void update_player_status(const glm::vec3& new_position, const glm::vec2& new_rotation, bool interpolate);
    void update_player_position(const glm::vec3& new_position);
    void update_player_orientation(const glm::vec2& new_orientation_deg);
    void rotate(const glm::vec2& angle_degrees);
    std::pair<bool, glm::vec3> collide(int height, const std::unordered_map<glm::ivec2, Chunk> &chunk_map);

    //void update_player(const Status& new_status, bool interpolate);
    //void interpolate_player();
    Block ray_hit(const Chunk& c, bool previous, int max_distance, int step = 32) const;
    //HitResult hit_test_face();
    bool insersects_block(int height, const glm::ivec3& block_pos) const;
    glm::ivec2 get_pq() const;
    void apply_movement(double delta_t, bool is_flying, bool jump_action,
                        const glm::vec2 &horizontal_motion, const std::unordered_map<glm::ivec2, Chunk> &chunk_map);
};


#endif //CPP_PLAYER_HPP
