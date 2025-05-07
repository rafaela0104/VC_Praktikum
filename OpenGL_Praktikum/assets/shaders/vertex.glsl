#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colorRGB;

out vec3 colorVS;

void main(){
    colorVS = colorRGB;
    gl_Position = vec4(pos, 1.0);

}
