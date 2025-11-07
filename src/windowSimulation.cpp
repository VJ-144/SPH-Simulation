#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "windowManager.h"
#include "windowSimulation.h"
#include "particleMethods.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering


        // constructor sets simulation asthetics
        windowSimulation::windowSimulation(float size_w, float size_h, std::string windowName) : windowManager(size_w, size_h, windowName)
        {    
            // Set particle asthetics for sim window
            glPointSize(10.0f);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            
            // scales width and length of inputs when plotting ( can effecively plot for range [100,100] )
            glScalef(1/100.0f, 1/100.0f, 1.0f); 
        }

        
        void windowSimulation::DrawParticles(std::vector<particle> plist)
        {
            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);
        
            glBegin(GL_POINTS);
            glColor3f(0.2f, 0.7f, 1.0f); // Blueish color
            for (auto &p : plist) 
            {
                glVertex2f(p.position[0], p.position[1]);      

            }
            glEnd();
            glfwSwapBuffers(window); 
        }
        

