#version 330 core

in vec2 fragment_uv;
in float diffuse_intensity;
in float fog_factor;
in float fog_height;

uniform float timer;
uniform float daylight;
uniform int ortho;

// textures
uniform sampler2D sampler;
uniform sampler2D sky_sampler;

// PI constant
uniform float PI;

out vec4 FragColor;

void main() {
    vec3 diffuse_color = vec3(texture2D(sampler, fragment_uv));
    bool no_color = (diffuse_color == vec3(1.0, 0.0, 1.0));
    bool is_cloud = (diffuse_color == vec3(1.0, 1.0, 1.0));

    if(no_color || (is_cloud && bool(ortho))){
        discard;
    }/*

    //float light_intensity = min(1.0, daylight);
    float light_intensity = 1.0;
    vec3 light = vec3(light_intensity);
    // diffuse color changes by day time
    diffuse_color *= light;

    vec3 sky_color = vec3(texture2D(sky_sampler, vec2(timer, fog_height)));
    // the more fog factor is high, the more diffuse_color goes towards sky_color
    diffuse_color = mix(diffuse_color, sky_color, fog_factor);*/

    FragColor = vec4(diffuse_color, 1.0);
}
