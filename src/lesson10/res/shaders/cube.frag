#version 330 core

in vec2 vTexCoords;
in vec3 vNormal;    // нормаль у світовому просторі (для освітлення)
in vec3 vFragPos;   // позиція фрагмента у світовому просторі (для освітлення)

uniform sampler2D uTexture0;

out vec4 FragColor;

void main() {
    vec4 color0 = texture(uTexture0, vTexCoords);
    FragColor = color0;  // колір отриманий з текстури
    // TODO: застосувати освітлення з використанням vNormal та vFragPos
}
