#version 330 core

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Input
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colorRGB;

// Output to Fragment Shader
out vec3 colorVS;

void main() {
    colorVS = colorRGB;

    // Vollständige Transformation: projection * view * model * pos
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
}
