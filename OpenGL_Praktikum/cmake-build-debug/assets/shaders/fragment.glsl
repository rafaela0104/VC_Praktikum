#version 330 core

in vec3 Normal;
in vec3 colorVS;

uniform float time;

out vec4 color;

void main() {
    // Basic lighting calculation
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(Normal);
    float diff = max(dot(normal, lightDir), 0.2); // 0.2 is ambient light

    vec3 finalColor = colorVS * diff;
    color = vec4(finalColor, 1.0);
}