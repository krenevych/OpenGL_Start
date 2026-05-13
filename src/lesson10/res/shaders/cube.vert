#version 330 core

in vec3 aPos;
in vec3 aNormal;

out vec3 vNormal;      // нормаль у світовому просторі
out vec3 vFragPos;     // позиція фрагмента у світовому просторі

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    // Трансформуємо позицію у світовий простір
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vFragPos = vec3(worldPos);

    // Normal matrix: transpose(inverse(uModel)) — для коректного перетворення нормалей
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    mat4 MVP = uProjection * uView * uModel;
    gl_Position = MVP * vec4(aPos, 1.0);
}
