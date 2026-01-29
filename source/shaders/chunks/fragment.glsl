#version 330 core

// inputs
//in vec3 GLOBAL_current_texture;

// texture uniforms
//uniform sampler2DArray GLOBAL_current_texture_unit;

// outputs
out vec4 GLOBAL_fragment_color;

// shader entry point
void main() {
    // setup fragment color
    GLOBAL_fragment_color = vec4(0.0, 1.0, 0.0, 1.0); //texture(GLOBAL_current_texture_unit, GLOBAL_current_texture);
    if (GLOBAL_fragment_color.a < 1.0f) {
        discard;
    }

    return;
}
