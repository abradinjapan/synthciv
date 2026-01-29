#version 330 core

// inputs
layout (location = 0) in vec3 GLOBAL_coordinates;
layout (location = 1) in vec3 GLOBAL_texture_layout;

// texture uniforms
//uniform sampler2DArray GLOBAL_current_texture_unit;

// outputs
//out vec3 GLOBAL_current_texture;

// shader entry point
void main() {
    // setup position
    gl_Position = vec4(GLOBAL_coordinates, 1.0f);

    return;
}
