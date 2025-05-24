#version 330 core

// Input attributes
layout(location = 0) in vec3 posVec;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec3 colorVec;

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


// Output to Fragment Shader
out vec3 colorVS;
out vec3 Normal;
out vec3 FragPos;


void main() {
    // Pass color to fragment shader
    colorVS = colorVec;

    // Transform normal to world space
    Normal = mat3(modelMatrix) * normalVec;

    FragPos = vec3(modelMatrix * vec4(posVec, 1.0));

    // Transform vertex position
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(posVec, 1.0);
}