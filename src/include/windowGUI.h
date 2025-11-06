#ifndef WINDOWGUI_H
#define WINDOWGUI_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering

#include "windowManager.h"

class windowGUI : public windowManager
{
    public:
            
        // constructor sets gui to window size
        windowGUI(float size_w, float size_h, const char* windowName);
        
        // switches frame context
        void makeCurrentContext();

        void createNewFrame();

        // starts draw
        void beginDraw(const char* windowName);

        // adds slider to window
        float addSlider(std::string varName, float variable, float vmax, float vmin);

        // ends draw
        void endDraw();

        // renders to window
        void Render();

};

#endif  // WINDOWGUI_H
