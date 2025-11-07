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

class windowSimulation : public windowManager
{
    public:


        
        // constructor sets simulation asthetics
        windowSimulation(float size_w, float size_h, std::string windowName);

        // draws particle positions
        void DrawParticles(std::vector<particle> plist);

};



#endif  // SIMULATIONWINDOW_H
