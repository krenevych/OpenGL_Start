#version 330 core

in vec4 aPos;

in vec2 aUV;
out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vTexCoords = aUV;
    vTexCoords.y = 1.0 - vTexCoords.y;

    mat4 MVP = uProjection * uView * uModel;
    gl_Position = MVP * aPos;
}