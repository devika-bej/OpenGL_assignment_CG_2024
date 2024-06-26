#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 rotMat;

flat out vec3 triColor;

void main() {
    gl_Position = projMat * viewMat * modelMat * rotMat * vec4(aPos, 1);
    triColor = aColor;
}