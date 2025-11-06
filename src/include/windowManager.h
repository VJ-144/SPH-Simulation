#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering

// popup window class
class windowManager 
{
  public:

    // class variables
    GLFWwindow *window;
    float size_w, size_h;
    const char* windowName;

    // constructor creates window
    windowManager(int size_w, int size_h, const char* windowName);

};



#endif  // WINDOWMANAGER_H
