#version 330 core

in vec2 fragment_uv;
in float diffuse_intensity;
in float fog_factor;
in float aoFrag;

uniform float timer;
uniform float daylight;
uniform vec3 sky_color;

// textures
uniform sampler2D sampler;

out vec4 FragColor;

void main() {
    vec3 diffuse_color = vec3(texture2D(sampler, fragment_uv));
    bool no_color = (diffuse_color == vec3(1.0, 0.0, 1.0));

    if(no_color){
        discard;
    }

    //float light_intensity = 1.0;
    diffuse_color *= diffuse_intensity;
    diffuse_color *= aoFrag;

    //vec3 sky_color = vec3(texture2D(sky_sampler, vec2(timer, fog_height)));
    // the more fog factor is high, the more diffuse_color goes towards sky_color
    diffuse_color = mix(diffuse_color, sky_color, fog_factor);

    FragColor = vec4(diffuse_color, 1.0);
}
