#version 330 core

// Input attributes
layout(location = 0) in vec3 vertexVec;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec3 colorVec;

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Output to Fragment Shader
out vec3 colorVS;
out vec3 Normal;

void main() {
    // Pass color to fragment shader
    colorVS = colorVec;

    // Transform normal to world space
    Normal = mat3(modelMatrix) * normalVec;

    // Transform vertex position
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexVec, 1.0);
}