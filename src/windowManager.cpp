#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "windowManager.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering


    // constructor creates window
    windowManager::windowManager(int size_w, int size_h, std::string windowName) 
    {

        // Initialise GLFW
        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            //return -1;
        }


        window = glfwCreateWindow(size_w, size_h, windowName.c_str(), NULL, NULL);
        if( !window ) 
        {
            fprintf( stderr, "Failed to open GLFW variables window.\n" );
            glfwTerminate();
            //return -1;
        } 

        // ensure we can capture inputs in window
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        // change context to this window
        glfwMakeContextCurrent(window);

        // initialise glew + legacy funcions
        glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK ) 
        {
            //fprintf(stderr, "Failed to initialize GLEW\n");
            throw std::invalid_argument("Failed to initialize GLEW");

        }


    }

