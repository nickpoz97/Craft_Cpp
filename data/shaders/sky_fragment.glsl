#version 330 core

in vec2 fragment_uv;

uniform sampler2D sampler;
uniform float timer;

out vec4 FragColor;

void main(){
    // texture depends on day time
    vec2 final_uv = vec2(timer, fragment_uv.t);
    FragColor = texture2D(sampler, final_uv);
}