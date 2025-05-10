#version 330 core

in vec3 colorVS;
uniform float time;

// Fragment shader output
out vec4 color;

void main(){
    float pulse = 0.5 + 0.5 * sin(time); // Wert zwischen 0 und 1
    vec3 animatedColor = colorVS * pulse; // Farbe wird heller und dunkler animiert
    color = vec4(animatedColor, 1.0);
}
