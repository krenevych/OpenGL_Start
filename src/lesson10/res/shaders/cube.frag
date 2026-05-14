#version 330 core

in vec3 vNormal;    // нормаль у світовому просторі
in vec3 vWorldPos;   // позиція фрагмента у світовому просторі

// Напрямлене світло (directional light)
uniform vec3 uLightDir;     // напрямок ВІД джерела світла (у світовому просторі)
uniform vec3 uLightColor;   // колір/інтенсивність світла

// Властивості матеріалу (Phong)
uniform vec3  uMaterialAmbient;   // амбієнтний колір матеріалу
uniform vec3  uMaterialDiffuse;   // дифузний колір матеріалу
uniform vec3  uMaterialSpecular;  // дзеркальний колір матеріалу
uniform float uShininess;         // показник блиску

// Позиція камери (для розрахунку specular)
uniform vec3 uCameraPos;

out vec4 FragColor;

void main() {
    // --- Ambient (фонове розсіяне світло) ---
    vec3 ambient = uLightColor * uMaterialAmbient;

    // --- Diffuse (дифузне відбиття) ---
    vec3 norm     = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDir);          // напрямок ДО джерела
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = uLightColor * diff * uMaterialDiffuse;

    // --- Specular (дзеркальне відбиття, Phong) ---
    vec3 viewDir    = normalize(uCameraPos - vWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);     // відбитий вектор
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
    vec3 specular   = uLightColor * spec * uMaterialSpecular;

    // --- Фінальний колір ---
    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
