#version 330 core

layout (location=0) in vec3 world_position;
layout (location=1) in vec2 vertex_uv;
layout (location=2) in vec3 normal;

uniform mat4 viewproj_matrix;
uniform vec3 camera_pos;
uniform float fog_distance;

// note: light direction is from target to source
const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));

out vec2 fragment_uv;
out float diffuse_intensity;
out float fog_factor;

void main() {
    gl_Position = viewproj_matrix * vec4(world_position, 1.0);
    fragment_uv = vertex_uv;
    // if angle is more than 90 degrees then face is at the opposite side of light
    diffuse_intensity = max(dot(normal, light_direction), 0.0);
    diffuse_intensity = min(diffuse_intensity + 0.3, 1.0);

    vec3 distance_vector = world_position - camera_pos;
    // fog get very strong with distance
    fog_factor = pow(clamp(length(distance_vector) / fog_distance, 0.0, 1.0), 3.0);
}
