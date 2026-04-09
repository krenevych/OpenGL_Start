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

########### LESSON 2 ##############
set(LESSON_TARGET lesson01)
set(LESSON_DIR src/${LESSON_TARGET})
add_executable(${LESSON_TARGET} ${LESSON_DIR}/main.cpp)
target_link_libraries(${LESSON_TARGET} ${libs})

########### LESSON 2 ##############
set(LESSON_TARGET lesson02)
set(LESSON_DIR src/${LESSON_TARGET})
add_executable(${LESSON_TARGET} ${LESSON_DIR}/main.cpp)
target_link_libraries(${LESSON_TARGET} ${libs})

