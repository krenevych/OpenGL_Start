cmake_minimum_required(VERSION 3.10)
project(OpenGL_Start)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)



find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

set(LESSON_TARGET lesson01)

add_executable(${LESSON_TARGET} src/lesson01/main.cpp)
target_link_libraries(${LESSON_TARGET} ${libs})

