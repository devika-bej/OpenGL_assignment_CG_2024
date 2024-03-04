#version 330 core
out vec4 FragColor;

in vec3 triColor;
uniform ivec2 Total;

void main() {
    FragColor = vec4(triColor, 1);
}