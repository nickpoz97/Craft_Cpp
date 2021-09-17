#version 330 core

in vec2 fragment_uv;

uniform sampler2D sampler;
uniform float timer;

void main(){
    // texture depends on day time
    vec2 final_uv = vec2(timer, fragment_uv.t);
    gl_FragColor = texture2D(sampler, final_uv);
}