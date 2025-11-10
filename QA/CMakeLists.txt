CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
project(QA_Test VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

# enable ctest
enable_testing()

# find libraries for viz
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


# --- source code to test & unit tests ---
add_executable( run_tests
                ../src/particleMethods.cpp
                ../src/windowManager.cpp
                ../src/windowGUI.cpp
                 ../src/windowSimulation.cpp

                test_particleMethods.cpp    )


# Link to libraries we want to use
target_link_libraries(  run_tests
                        GLEW::GLEW
                        glfw
                        OpenGL::GL
                        imgui
                        gtest
                        gtest_main
                        pthread     )


# Include directories so tests can find header files
target_include_directories( run_tests PUBLIC
                            ../include
                            ../src
                            ../libs/glfw
                            ../libs/imgui     
                            .     )

# Automatically discover and register all tests
include(GoogleTest)
gtest_discover_tests(run_tests)

