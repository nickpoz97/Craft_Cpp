#version 330 core

in vec2 fragment_uv;

uniform sampler2D sampler;

out vec4 FragColor;

void main() {
    vec4 color = texture2D(sampler, fragment_uv);
    color.a = max(color.a, 0.4);

    FragColor = color;
}
