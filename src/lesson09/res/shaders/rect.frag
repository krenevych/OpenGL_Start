#version 330 core

in vec4 vColor;


out vec4 FragColor;

void main() {
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // колір отриманий з текстури
    FragColor = vColor;  // колір отриманий з текстури
}