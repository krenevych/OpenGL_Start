#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"
#include "texture.h"


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
    auto height =920;
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

    glClearColor(1.0, 1.0, 1.0, 1.0);

    std::string vertexShaderName = "res/shaders/cube.vert";
    std::string fragmentShaderName = "res/shaders/cube.frag";
    GLuint shaderProgram = createProgram(
        vertexShaderName,
        fragmentShaderName);

    float vertices[] = {
        // -------- Передня грань --------
        -1.0f, -1.0f, -1.0f,    /* текстурні координати */  0.0, 0.0,
         1.0f, -1.0f, -1.0f,    /* текстурні координати */  1.0, 0.0,
         1.0f,  1.0f, -1.0f,    /* текстурні координати */  1.0, 1.0,
        -1.0f,  1.0f, -1.0f,    /* текстурні координати */  0.0, 1.0,

       // -------- Задня грань --------
        -1.0f, -1.0f, 1.0f,    /* текстурні координати */  0.0, 0.0,
         1.0f, -1.0f, 1.0f,    /* текстурні координати */  1.0, 0.0,
         1.0f,  1.0f, 1.0f,    /* текстурні координати */  1.0, 1.0,
        -1.0f,  1.0f, 1.0f,    /* текстурні координати */  0.0, 1.0,

       // -------- Ліва грань (синя) --------
        -1.0f, -1.0f, -1.0f,    /* текстурні координати */  0.0, 0.0,
        -1.0f,  1.0f, -1.0f,    /* текстурні координати */  0.0, 1.0,
        -1.0f,  1.0f,  1.0f,    /* текстурні координати */  1.0, 1.0,
        -1.0f, -1.0f,  1.0f,    /* текстурні координати */  1.0, 0.0,

       // -------- Права грань --------
         1.0f, -1.0f, -1.0f,    /* текстурні координати */  0.0, 0.0,
         1.0f,  1.0f, -1.0f,    /* текстурні координати */  0.0, 1.0,
         1.0f,  1.0f,  1.0f,    /* текстурні координати */  1.0, 1.0,
         1.0f, -1.0f,  1.0f,    /* текстурні координати */  1.0, 0.0,

       // -------- Нижня грань --------
        -1.0f, -1.0f, -1.0f,    /* текстурні координати */  0.0, 0.0,
        -1.0f, -1.0f,  1.0f,    /* текстурні координати */  1.0, 0.0,
         1.0f, -1.0f,  1.0f,    /* текстурні координати */  1.0, 1.0,
         1.0f, -1.0f, -1.0f,    /* текстурні координати */  0.0, 1.0,

       // -------- Верхня грань --------
        -1.0f,  1.0f, -1.0f,    /* текстурні координати */  0.0, 0.0,
         1.0f,  1.0f, -1.0f,    /* текстурні координати */  1.0, 0.0,
         1.0f,  1.0f,  1.0f,    /* текстурні координати */  1.0, 1.0,
        -1.0f,  1.0f,  1.0f,    /* текстурні координати */  0.0, 1.0,
   };

    unsigned int indices[] = {
        0, 2, 1,    2, 0, 3,       // передня
        4, 5, 6,    6, 7, 4,       // задня
        8, 10, 9,   10,8,11,       // ліва
       12,13,14,   14,15,12,       // права
       16,18, 17,  18,16,19,       // нижня
       20,22,21,   22,20,23        // верхня
    };

    GLuint VBO, indexBuffer; // data - ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,                  // знайдена командою glGetAttribLocation позиція атрибуту у шейдері
        3,                  // 3 компоненти: x, y, z
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        5 * sizeof(float),  // stride: 5 float-а на вершину
        (void*)0            // offset: починаємо з 0
    );
    glEnableVertexAttribArray(posAttribLocation);

    GLuint textureCoordsAttribLocation = glGetAttribLocation(shaderProgram, "aUV");
    glVertexAttribPointer(
        textureCoordsAttribLocation,                  // знайдена командою glGetAttribLocation позиція атрибуту у шейдері
        2,                  // 2 компоненти: u, v
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        5 * sizeof(float),  // stride: 5 float-а на вершину
        (void*)(3 * sizeof(float))        // offset
    );
    glEnableVertexAttribArray(textureCoordsAttribLocation);

    glBindVertexArray(0); // деактивувати VAO

    GLint model_loc = glGetUniformLocation(shaderProgram, "uModel");
    GLint view_loc = glGetUniformLocation(shaderProgram, "uView");
    GLint proj_loc = glGetUniformLocation(shaderProgram, "uProjection");

    float t = 0.0f;
    float deltaTime = 1.0f / 60.0f;

    auto model = glm::mat4(1.0f);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 2.0f, -5.0f), // позиція камери
        glm::vec3(0.0f, 0.0f, 0.0f), // куди дивимось
        glm::vec3(0.0f, 1.0f, 0.0f) // вектор вгору
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CCW); // GL_CW.
    // glFrontFace(GL_CW); // GL_CW.

    unsigned int texture0 = loadTexture("res/textures/brick.jpg");
    GLint texture0_loc = glGetUniformLocation(shaderProgram, "uTexture0");

    /* Loop until the user closes the window */
    do
    {
        t += deltaTime;
        if (t >= 1.0f || t <= 0.0f) {
            deltaTime = -deltaTime;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаємо не лише полотно, на якому малюємо, але й буфер глибини
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(texture0_loc, 0);

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));


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
    glDeleteTextures(1, &texture0);

    glfwTerminate();
    return 0;
}
