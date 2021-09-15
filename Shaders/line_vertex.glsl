#version 330 core

layout (location=0) in vec3 world_position;

uniform mat4 viewproj_matrix;

void main() {
    gl_Position = viewproj_matrix * vec4(world_position, 1.0);
}
