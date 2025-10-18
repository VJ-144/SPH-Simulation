#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <cmath>
#include <vector>

// project files
#include "particleMethods.cpp"
#include "viz.cpp"
// this file will hold the main loop for the liquid simulation.


#define screenWidth 5
#define screenHeight 5

using namespace std;
const double h = 0.05;

int main(int argc, char** argv) {

    const int nParticles = 4;
    vector<particle> plist;

    vector<double> pos = {0.0, 0.0};
    vector<double> vel = {0.0, 0.0};
  
    //particle p1; 
    //p1.Init(pos, vel);
    //printf("Initialised vel: %f %f \n", p1.velocity[0], p1.velocity[1]);

    for (int i=0; i<nParticles; i++) {
            
        // particle initial conditions
        pos[0] += 0.025;
        if (i%5==0) {
            pos[0] = 0.0;    
            pos[1] += 0.050;
        }
   
        // set initial particle arguments and smoothing radius 
        particle p(pos, vel, i, nParticles);
        //p.set_SR(h);

        printf("Initialised pos: %f %f \n", p.position[0], p.position[1]);

        plist.push_back(p);

    }


    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 1000, 800, "Fluid Simulation", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    } 

    // Ensure we can capture the escape key being pressed below 
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); 
    glfwMakeContextCurrent(window);

    // Initialize GLEW
     glewExperimental=true; // Needed in core profile
    if (!glewInit()) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    //point size in normalised coordinates
    double normX_ps = (1.0/800) * 20.0;
    double normY_ps = (1.0/1000) * 20.0;

    // Set OpenGL state
    glPointSize(10.0f); // Make particles more visible
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    


    // Main loop
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ) {

 
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

        // add fluid-fluid collisions
        for (particle &p : plist) {
                
            // calculating force per particle    
            //p.ff_collisions(plist);
            //p.update_pos();
            
            //printf("index %d | px:%f | py:%f \n", p.index_,  p.position[0], p.position[1]);
        }

        // clears color of draw bit
        glClear(GL_COLOR_BUFFER_BIT);

        // Render particles
        glBegin(GL_POINTS);
        glColor3f(0.2f, 0.7f, 1.0f); // Blueish color
        for (auto &p : plist) {
            glVertex2f(p.position[0], p.position[1]);      

        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();




  return 0;
}
