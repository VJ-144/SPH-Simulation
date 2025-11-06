#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "include/particleMethods.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering

// 2D particle class
class windowManager 
{
  public:

    // class variables
    GLFWwindow *window;
    float size_w, size_h;
    const char* windowName;


    // constructor creates window
    windowManager(int size_w, int size_h, const char* windowName) 
    {

        // Initialise GLFW
        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            //return -1;
        }


        window = glfwCreateWindow(size_w, size_h, windowName, NULL, NULL);
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
};


class simulationWindow : public windowManager
{
    public:
        
        // constructor sets simulation asthetics
        simulationWindow(float size_w, float size_h, const char* windowName) : windowManager(size_w, size_h, windowName)
        {    
            // Set particle asthetics for sim window
            glPointSize(10.0f);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            
            // scales width and length of inputs when plotting ( can effecively plot for range [100,100] )
            glScalef(1/100.0f, 1/100.0f, 1.0f); 
        }

        
        void DrawParticles(std::vector<particle> plist)
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
        
        }
        


};



class windowGUI : public windowManager
{
    public:
            
        // constructor    
        windowGUI(float size_w, float size_h, const char* windowName) : windowManager(size_w, size_h, windowName)
        {
            IMGUI_CHECKVERSION();

            // create context for imgui
            ImGui::CreateContext(); 

            // retrieve reference to io configuration settings for gui
            ImGuiIO& io = ImGui::GetIO();
            
            // initialise glfw for openGL in imgui
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            
            // initialise openGL for imgui
            ImGui_ImplOpenGL3_Init();

            // set gui colour scheme
            ImGui::StyleColorsDark();
        }


        void makeCurrentContext()
        {
            glfwMakeContextCurrent(window);
        }


        void createNewFrame()
        {
            // clear frame
            glClear(GL_COLOR_BUFFER_BIT);
    
            // get current frame buffer for new buffer
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            // create new frame buffer
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // places imgui frame/buffer at top-left corner
            ImGui::SetNextWindowPos( ImVec2(0, 0) );
            ImGui::SetNextWindowSize( ImVec2((float)display_w, (float)display_h) );
        }

        void beginDraw(const char* windowName)
        {

            ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        }

        // adds slider to window
        float addSlider(std::string varName, float variable, float vmax, float vmin)
        {
            float centerVal{variable};
            float maxVal{vmax};
            float minVal{vmin};

            // unique id for imgui
            std::string str_slide_id = "###Slide_";
            str_slide_id += varName;

            std::string str_input_id = "###Input_";
            str_input_id += varName;

            ImGui::Text("%s", varName.c_str());
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.15f);
        
            // add typing box for input variable - only changes slider value (+min/max) if new value typed in
            bool changedFloat = ImGui::InputFloat(str_slide_id.c_str(), &variable, 0.0f, 0.0f, "%.3f");
            if (changedFloat == true) 
            {
                centerVal = variable;
                maxVal = centerVal + vmax;
                minVal = std::max(centerVal - vmin, (float)0.0);
            }

            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

            // add slider box for input variable - only updates variable input box if slider is changed
            bool changedSlider = ImGui::SliderFloat(str_input_id.c_str(), &centerVal, (float)vmin, (float)vmax, "%.3f", ImGuiSliderFlags_NoInput);
            if (changedSlider == true) {
                variable = centerVal;
            }
    
            return variable;
        }


        void endDraw()
        {
            ImGui::End();
        }


        void Render()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
            glfwSwapBuffers(window);
        }

};
