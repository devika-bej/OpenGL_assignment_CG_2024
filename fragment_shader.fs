#version 330 core
out vec4 FragColor;

in vec3 triColor;

void main() {
    FragColor = vec4(triColor, 1);
}