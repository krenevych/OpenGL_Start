#version 330 core

in vec2 vTexCoords;
in vec3 vNormal;    // нормаль у світовому просторі
in vec3 vFragPos;   // позиція фрагмента у світовому просторі

// Текстура
uniform sampler2D uTexture0;

// Напрямлене світло (directional light)
uniform vec3 uLightDir;     // напрямок ВІД джерела світла (у світовому просторі)
uniform vec3 uLightColor;   // колір/інтенсивність світла

// Компоненти Phong-моделі
uniform float uAmbientStrength;     // сила амбієнтного освітлення
uniform float uSpecularStrength;    // сила дзеркального відбиття
uniform float uShininess;           // показник блиску (чим більше — тим вужче пляма)

// Позиція камери (для розрахунку specular)
uniform vec3 uViewPos;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(uTexture0, vTexCoords);

    // --- Ambient (фонове розсіяне світло) ---
    vec3 ambient = uAmbientStrength * uLightColor;

    // --- Diffuse (дифузне відбиття) ---
    vec3 norm     = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDir);          // напрямок ДО джерела
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * uLightColor;

    // --- Specular (дзеркальне відбиття, Phong) ---
    vec3 viewDir    = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);     // відбитий вектор
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
    vec3 specular   = uSpecularStrength * spec * uLightColor;

    // --- Фінальний колір ---
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    //    vec3 lighting = ambient + diffuse + specular;
    lighting += ambient;
    lighting += diffuse;
    lighting += specular;
    FragColor = vec4(lighting * vec3(texColor), texColor.a);
}
