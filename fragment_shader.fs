#version 330 core
flat in vec3 triColor;

out vec4 fragColor;

void main() {
    fragColor = vec4(triColor, 1);
}