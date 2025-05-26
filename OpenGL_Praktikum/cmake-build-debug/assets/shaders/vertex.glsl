#version 330 core

// Input attributes
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;


// Output to Fragment Shader
out vec3 colorVS;
out vec3 FragPos;
out vec3 normal;
out vec3 lightDir;
out vec3 viewDir;


void main() {

    vec4 n = vec4(vertexNormal, 1.0);
    mat4 normalMat = transpose(inverse(viewMatrix * modelMatrix));
    normal = (normalMat * n).xyz;

    vec4 lp = vec4(viewMatrix * vec4(lightPos, 1.0));
    vec4 P = viewMatrix * modelMatrix * vec4(vertex, 1.0);
    lightDir = (lp - P).xyz;

    colorVS = vertexColor;

    viewDir = (-P).xyz;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
}