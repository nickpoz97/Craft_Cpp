#version 330 core

layout (location=0) in vec3 world_position;
layout (location=1) in vec2 vertex_uv;
layout (location=2) in vec3 normal;

uniform mat4 viewproj_matrix;
uniform vec3 camera_pos;
uniform float fog_distance;
uniform int ortho;
uniform float PI = 3.14159265;

// note: light direction is from target to source
const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));

out vec2 fragment_uv;
out float diffuse_intensity;
out float fog_factor;
out float fog_height;

void main() {
    gl_Position = viewproj_matrix * vec4(world_position, 1.0);
    fragment_uv = vertex_uv;
    // if angle is more than 90 degrees then face is at the opposite side of light
    //diffuse_intensity = max(0.0, dot(normal, light_direction));
    diffuse_intensity = 1.0;

    if(bool(ortho)){
        fog_factor = 0.0;
        fog_height = 0.0;
    }
    else{
        vec3 distance_vector = world_position - camera_pos;
        // fog get very strong with distance
        fog_factor = pow(clamp(length(distance_vector) / fog_distance, 0.0, 1.0), 4.0);
        float vertical_distance = distance_vector.y;
        float horizontal_distance = length(distance_vector.xz);
        // the value fog_height is the pitch
        // since at is between -PI/2 and +PI/2 the value will be between 0 and 1
        fog_height = (atan(vertical_distance / horizontal_distance) + PI/2) / PI;
    }
}
