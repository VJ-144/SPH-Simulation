#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

// 2D particle class
class plotManager {
  public:

    int err;

    // initialises window for visualisation
    int InitLibs() {

      // Initialise GLFW
      glewExperimental = true; // Needed for core profile
      if( !glfwInit() )
      {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
      }

      // giving OpenCL hints for setup
      glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

      // Initialise GLEW
      //glewExperimental=true; // Needed in core profile
      if (!glewInit()) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
      }

      return 0;
    }


    int createWindow() {


      return 0;
    }

    int terminateWindow(){
      glfwTerminate();
      return 0;
    }


};
