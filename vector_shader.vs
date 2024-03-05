#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform int tilt;
uniform vec3 objectTranslate;

flat out vec3 triColor;

void main() {
    vec4 translatedPos = vec4(aPos.x + objectTranslate.x, aPos.y + objectTranslate.y, aPos.z + objectTranslate.z, 1);
    float cosTilt = cos(tilt);
    float sinTilt = sin(tilt);
    mat4 rotMat = mat4(cosTilt, 0, sinTilt, 0, 0, 1, 0, 0, -sinTilt, 0, cosTilt, 0, 0, 0, 0, 1);
    gl_Position = translatedPos * rotMat;
    triColor = aColor;
}
