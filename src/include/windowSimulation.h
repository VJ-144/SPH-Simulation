#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering

#include "windowManager.h"
#include "particleMethods.h"

class simulationWindow : public windowManager
{
    public:
        
        // constructor sets simulation asthetics
        simulationWindow(float size_w, float size_h, const char* windowName);

        // draws particle positions
        void DrawParticles(std::vector<particle> plist);

};



#endif  // SIMULATIONWINDOW_H
