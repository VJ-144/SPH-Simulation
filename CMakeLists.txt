CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(liquid VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable( liquid 
                src/mainLoop.cpp 
                src/particleMethods.cpp
                src/viz.cpp       )

target_link_libraries(  liquid
                        GLEW::GLEW
                        glfw
                        OpenGL::GL )


