cmake_minimum_required(VERSION 3.15)
project(PacmanCG)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS ./include)
set(GLEW_H /usr/local/Cellar/glew/2.1.0_1/include)
set(GLFW_H /usr/local/Cellar/glfw/3.3.2/include)
set(GLM_H /usr/local/Cellar/glm/0.9.9.5/include)
include_directories(${GLEW_H} ${GLFW_H} ${GLM_H} ${HEADERS})

set(GLEW_LINK /usr/local/Cellar/glew/2.1.0_1/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

#aux_source_directory(./src/ DIR_SRCS)

set(SOURCE_FILES main.cpp src/game.cpp src/game_object.cpp src/shader.cpp src/texture.cpp include/resource_manager.h src/resource_manager.cpp src/sprite_renderer.cpp src/stb_image_wrapper.cpp src/game_level.cpp src/monster.cpp src/ball.cpp)
add_executable(PacmanCG ${SOURCE_FILES})

if (APPLE)
    target_link_libraries(PacmanCG "-framework OpenGL")
    target_link_libraries(PacmanCG "-framework GLUT")
endif()
