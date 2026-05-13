#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main() {
    vec4 color0 = texture(uTexture0, vTexCoords);
    FragColor = color0;  // колір отриманий з текстури
}