#version 330 core

in vec4 aPos;
in vec4 aColor;

out vec4 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vColor = aColor;

    mat4 MVP = uProjection * uView * uModel;
    gl_Position = MVP * aPos;
}