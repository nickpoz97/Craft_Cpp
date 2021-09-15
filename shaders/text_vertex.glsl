#version 330 core

layout (location=0) in vec3 world_position;
layout (location=1) in vec2 vertex_uv;

uniform mat4 viewproj_matrix;

out vec2 fragment_uv;

void main() {
    gl_Position = viewproj_matrix * vec4(world_position, 1.0);
    fragment_uv = vertex_uv;
}
