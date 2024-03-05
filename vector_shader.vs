#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform vec3 objectTranslate;

flat out vec3 triColor;

void main() {
    gl_Position = vec4(aPos.x + objectTranslate.x, aPos.y + objectTranslate.y, aPos.z + objectTranslate.z, 1);
    triColor = aColor;
}
