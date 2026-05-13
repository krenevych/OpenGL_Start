#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto width = 1280;
    auto height = 920;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Cube", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSwapInterval(1);  // синхронізує рендер-цикл з частотою екрану

    // glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    std::string vertexShaderName = "res/shaders/cube.vert";
    std::string fragmentShaderName = "res/shaders/cube.frag";
    GLuint shaderProgram = createProgram(
        vertexShaderName,
        fragmentShaderName);

    // Формат на вершину: position(3) + normal(3) = 6 floats
    float vertices[] = {
        // -------- Передня грань (нормаль: 0, 0, -1) --------
        -1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,

        // -------- Задня грань (нормаль: 0, 0, 1) --------
        -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,

        // -------- Ліва грань (нормаль: -1, 0, 0) --------
        -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,

        // -------- Права грань (нормаль: 1, 0, 0) --------
         1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,    1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,    1.0f,  0.0f,  0.0f,

        // -------- Нижня грань (нормаль: 0, -1, 0) --------
        -1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,

        // -------- Верхня грань (нормаль: 0, 1, 0) --------
        -1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,
    };

    unsigned int indices[] = {
        0, 2, 1,    2, 0, 3,       // передня
        4, 5, 6,    6, 7, 4,       // задня
        8, 10, 9,   10,8,11,       // ліва
       12,13,14,   14,15,12,       // права
       16,18, 17,  18,16,19,       // нижня
       20,22,21,   22,20,23        // верхня
    };

    GLuint VBO, indexBuffer;
    GLuint VAO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // stride: 6 float-ів на вершину (3 позиція + 3 нормаль)
    const int STRIDE = 6 * sizeof(float);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,          // позиція атрибуту у шейдері
        3,                          // 3 компоненти: x, y, z
        GL_FLOAT,                   // тип даних
        GL_FALSE,                   // не нормалізувати
        STRIDE,                     // stride: 6 float-ів на вершину
        (void*)0                    // offset: позиція починається з 0
    );
    glEnableVertexAttribArray(posAttribLocation);

    GLuint normalAttribLocation = glGetAttribLocation(shaderProgram, "aNormal");
    glVertexAttribPointer(
        normalAttribLocation,       // позиція атрибуту у шейдері
        3,                          // 3 компоненти: nx, ny, nz
        GL_FLOAT,                   // тип даних
        GL_FALSE,                   // не нормалізувати
        STRIDE,                     // stride: 6 float-ів на вершину
        (void*)(3 * sizeof(float))  // offset: нормаль після 3 float-ів позиції
    );
    glEnableVertexAttribArray(normalAttribLocation);

    glBindVertexArray(0); // деактивувати VAO

    GLint model_loc = glGetUniformLocation(shaderProgram, "uModel");
    GLint view_loc  = glGetUniformLocation(shaderProgram, "uView");
    GLint proj_loc  = glGetUniformLocation(shaderProgram, "uProjection");

    // Uniform-локації для напрямленого світла
    GLint lightDir_loc   = glGetUniformLocation(shaderProgram, "uLightDir");
    GLint lightColor_loc = glGetUniformLocation(shaderProgram, "uLightColor");
    GLint viewPos_loc    = glGetUniformLocation(shaderProgram, "uViewPos");

    // Uniform-локації для матеріалу
    GLint materialAmbient_loc  = glGetUniformLocation(shaderProgram, "uMaterialAmbient");
    GLint materialDiffuse_loc  = glGetUniformLocation(shaderProgram, "uMaterialDiffuse");
    GLint materialSpecular_loc = glGetUniformLocation(shaderProgram, "uMaterialSpecular");
    GLint shininess_loc        = glGetUniformLocation(shaderProgram, "uShininess");

    float t = 0.0f;
    float deltaTime = 1.0f / 60.0f;

    auto model = glm::mat4(1.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, -5.0f);

    glm::mat4 view = glm::lookAt(
        cameraPos,                    // позиція камери
        glm::vec3(0.0f, 0.0f, 0.0f), // куди дивимось
        glm::vec3(0.0f, 1.0f, 0.0f)  // вектор вгору
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Параметри напрямленого світла
    glm::vec3 lightDir   = glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)); // напрямок ВІД джерела
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);                  // біле світло

    // Матеріал: copper — http://devernay.free.fr/cours/opengl/materials.html
    glm::vec3 materialAmbient = glm::vec3(0.19125, 0.0735, 0.0225);
    glm::vec3 materialDiffuse = glm::vec3(0.7038, 0.27048, 0.0828);
    glm::vec3 materialSpecular = glm::vec3(0.256777, 0.137622, 0.086014);
    float     shininess        = 0.1f * 128.0f;  // = 27.9f

    // // Матеріал: silver — http://devernay.free.fr/cours/opengl/materials.html
    // glm::vec3 materialAmbient = glm::vec3(0.19225	,0.19225	,0.19225);
    // glm::vec3 materialDiffuse = glm::vec3(0.50754	,0.50754	,0.50754);
    // glm::vec3 materialSpecular = glm::vec3(0.508273	,0.508273,	0.508273);
    // float     shininess        = 0.4f * 128.0f;  // = 27.9f

    // Матеріал: gold — http://devernay.free.fr/cours/opengl/materials.html
    // glm::vec3 materialAmbient = glm::vec3(0.24725,	0.1995	,0.0745);
    // glm::vec3 materialDiffuse = glm::vec3(0.75164,	0.60648	,0.22648);
    // glm::vec3 materialSpecular = glm::vec3(0.628281,	0.555802,	0.366065);
    // float     shininess        = 0.4f * 128.0f;  // = 27.9f

    // Матеріал: obsidian (obsidian) — http://devernay.free.fr/cours/opengl/materials.html
    // glm::vec3 materialAmbient = glm::vec3(0.05375, 0.05, 0.06625);
    // glm::vec3 materialDiffuse = glm::vec3(0.18275, 0.17, 0.22525);
    // glm::vec3 materialSpecular = glm::vec3(0.332741, 0.328634, 0.346435);
    // float     shininess        = 0.3f * 128.0f;  // = 27.9f

    // Матеріал: бронза (bronze) — http://devernay.free.fr/cours/opengl/materials.html
    // glm::vec3 materialAmbient = glm::vec3(0.2125, 0.1275, 0.054);
    // glm::vec3 materialDiffuse = glm::vec3(0.714, 0.4284, 0.18144);
    // glm::vec3 materialSpecular = glm::vec3(0.393548, 0.271906, 0.166721);
    // float     shininess        = 0.2f * 128.0f;  // = 27.9f

    // Матеріал: Brass (латунь) — http://devernay.free.fr/cours/opengl/materials.html
    // glm::vec3 materialAmbient  = glm::vec3(0.329412f, 0.223529f, 0.027451f);
    // glm::vec3 materialDiffuse  = glm::vec3(0.780392f, 0.568627f, 0.113725f);
    // glm::vec3 materialSpecular = glm::vec3(0.992157f, 0.941176f, 0.807843f);
    // float     shininess        = 0.21794872f * 128.0f;  // = 27.9f

    glUseProgram(shaderProgram);
    glUniform3fv(lightDir_loc,          1, glm::value_ptr(lightDir));
    glUniform3fv(lightColor_loc,        1, glm::value_ptr(lightColor));
    glUniform3fv(viewPos_loc,           1, glm::value_ptr(cameraPos));
    glUniform3fv(materialAmbient_loc,   1, glm::value_ptr(materialAmbient));
    glUniform3fv(materialDiffuse_loc,   1, glm::value_ptr(materialDiffuse));
    glUniform3fv(materialSpecular_loc,  1, glm::value_ptr(materialSpecular));
    glUniform1f(shininess_loc,          shininess);

    bool isPaused     = false;  // чи зупинена анімація
    bool prevPKeyDown = false;  // попередній стан клавіші P (для детекції одиночного натискання)

    /* Loop until the user closes the window */
    do
    {
        t += deltaTime;
        if (t >= 1.0f || t <= 0.0f) {
            deltaTime = -deltaTime;
        }

        // Перемикання паузи по клавіші P / p (спрацьовує лише на момент натискання)
        bool currPKeyDown = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
        if (currPKeyDown && !prevPKeyDown) {
            isPaused = !isPaused;
        }
        prevPKeyDown = currPKeyDown;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Обертаємо куб лише якщо анімація не на паузі
        if (!isPaused) {
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc,  1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
