#version 330 core
out vec4 FragColor;

uniform ivec2 Total;

void main() {
    float r, g, b;
    r = 1;
    if(gl_PrimitiveID < Total.x) {
        g = 0;
    } else if(gl_PrimitiveID < 2 * Total.x) {
        g = 1;
    } else {
        int index = gl_PrimitiveID - 2 * Total.x;
        g = (index / 2) / Total.x;
    }
    b = 1;
    FragColor = vec4(r, g, b, 1);
}