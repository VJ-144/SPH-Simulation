#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"       // ImGui ↔ GLFW integration
#include "backends/imgui_impl_opengl3.h"    // ImGui ↔ OpenGL3 rendering

#include <math.h>
#include <cmath>
#include <vector>

// project files
#include "particleMethods.h"

#include "windowManager.h"
#include "windowGUI.h"
#include "windowSimulation.h"

// defining static class members - allocated in data segment memory  (static class variables)
double particle::SR = 14.0; 
double particle::dt = 0.2;
double particle::mass = 1.0;
double particle::kappa = 1000.0;                // stiffness parameter - gas constant
double particle::rest_density = 0.05;           // rest density (target density of the fluid when at equilibrium)
double particle::nu = 1.0;                      // viscosity strength


int main(int argc, char** argv) {

    const int nParticles = 150;
    vector<particle> plist;

    vector<double> pos = {0.0, 0.0};
    vector<double> vel = {0.0, 0.0};
  

    for (int i=0; i<nParticles; i++) {

        // particle initial conditions
        int x = -100 + rand() % (100 + 100 + 1);
        int y = -100 + rand() % (100 + 100 + 1);

        pos[0] = (double)x;
        pos[1] = (double)y;

        // set initial particle arguments and smoothing radius 
        particle p(pos, vel, i, nParticles);

        plist.push_back(p);

    }

    // local simulation parameters
    float SR = 14.0; 
    float dt = 0.2;
    float mass = 1.0;
    float kappa = 1000.0;                // stiffness parameter - gas constant
    float rest_density = 0.05;           // rest density (target density of the fluid when at equilibrium)
    float nu = 1.0;                      // viscosity strength


    float centerVal{kappa};
    float minVal{max(centerVal-(float)5000.0, (float)0.0)}, maxVal{centerVal+(float)5000.0};


    // experimenting with class based gui
    windowGUI gui(600, 200, "Parameter Controls");
    windowSimulation simWindow((float)800, (float)600, "SPH Simulation");

    // Main loop
    while( glfwGetKey(gui.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(gui.window) == 0 ) {

        // step 0: update simulation input parameters from gui
        particle::update_parameters( (double)dt,
                                     (double)SR,
                                     (double)mass,
                                     (double)kappa,
                                     (double)rest_density,
                                     (double)nu             );
        


        // step 1: find all neighbour particles
        for (particle &p : plist){
            
            // calc nearest particles & stores as class variable
           p.calc_NNindex(plist);
        }


        // step 2: calculate density per particle
        for (particle &p : plist ){
           
            // particle calculates its own density
            p.calc_density(plist);
            p.calc_pressure();
            
        }

        // step 3: calculate forces
        for (particle &p : plist) {

            // calculating force per particle    
            p.calc_force(plist);

        }

        // step 4: update vel & pos
        for (particle &p : plist) {

            // calculating force per particle    
            p.update_vel();
            p.update_pos();
                        
            //printf("index %d | px:%f | py:%f \n", p.index_,  p.position[0], p.position[1]);
        }


        // check for new inputs
        glfwPollEvents();
        

        // using gui class for DearImGUi
        gui.makeCurrentContext();
        gui.createNewFrame();
        gui.beginDraw("Variable Parameters v2");

        kappa = gui.addSlider("Kappa:", (float)kappa, (float)2000.0, (float)0.0);
        SR = gui.addSlider("SR:   ", (float)SR, (float)50.0, (float)0.0);
        dt = gui.addSlider("dt:   ", (float)dt, (float)1.0, (float)0.0);
        mass = gui.addSlider("mass: ", (float)mass, (float)2.0, (float)0.0);
        rest_density = gui.addSlider("rho_0:", (float)rest_density, (float)0.5, (float)0.0);
        nu = gui.addSlider("Nu:   ", (float)nu, (float)2.0, (float)0.0);
        
        gui.endDraw();
        gui.Render();
   


        // render particles
        simWindow.DrawParticles(plist);

    }

    glfwTerminate();




  return 0;
}
