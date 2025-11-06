CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
project(liquid VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

# OpenGL and GLEW installed globally on system
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(glfw3 REQUIRED)


# --- Build options for GLFW ---
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# --- Force X11 and disable Wayland ---
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

# --- Add GLFW as a subproject---
add_subdirectory(libs/glfw)

# ------ Add ImGUI ----------
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
file(   GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp   
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp   )
    

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories( imgui PUBLIC
                            ${IMGUI_DIR}
                            ${IMGUI_DIR}/backends   )
target_link_directories(imgui PUBLIC glfw GLEW::GLEW OpenGL::GL)
# ---------------------------

add_executable( run 
                src/mainLoop.cpp 
                src/particleMethods.cpp
                src/windowManager.cpp )


target_link_libraries(  run
                        GLEW::GLEW
                        glfw
                        OpenGL::GL
                        imgui        )

target_link_directories( run PUBLIC libs/glfw/include )
