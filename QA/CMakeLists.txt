# --- Add test executable 1 ---
add_executable( run_tests
                ../src/particleMethods.cpp
                ../src/windowManager.cpp
                ../src/windowGUI.cpp
                 ../src/windowSimulation.cpp

                test_particleMethods.cpp    )


# Link to your library and GoogleTest
target_link_libraries(  run_tests
                        imgui
                        gtest
                        gtest_main
                        pthread     )


# Include directories so tests can find headers
target_include_directories( run_tests PUBLIC
                            ../include
                            ../src
                            ../libs/glfw
                            ../libs/imgui     
                            .     )

# Automatically discover and register all tests in this executable
include(GoogleTest)
gtest_discover_tests(run_tests)

