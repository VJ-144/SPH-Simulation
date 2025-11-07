#ifndef PARTICLEMETHODS_H
#define PARTICLEMETHODS_H

#include <stdlib.h>
#include <bits/stdc++.h>
#include <vector>
#include <string>


using namespace std;

// 2D particle class
class particle {
  public:

    // particle label
    int index_;
    int nParticles_;

    // list of all neighbor particles
    int *neighbor_index;

    // static belongs to the class - not specific to the object
    static double SR;
    static double dt;
    static double mass;
    static double kappa;                // stiffness parameter - gas constant
    static double rest_density;         // rest density (target density of the fluid when at equilibrium)
    static double nu;                   // viscosity strength

    vector<double> position{0.0, 0.0};
    vector<double> velocity{0.0, 0.0};
    vector<double> acceleration{0.0, 0.0};

    // calculated properties
    double pressure{0.0};
    double density{0.0};
    vector<double> force = {0.0, 0.0};

    // need to fix how destructor is implemented
    // cause we store all particles in a vector the destructor is called as the vector reallocates space for new elements
//    ~particle() {
//        printf("particle destroyed\n");
//    }
    
    // constructor
    particle();

    // constructor
    particle( vector<double> pos, vector<double> vel, int index, int nParticles);

    // updates simulation control parameters via gui
    static void update_parameters(double newDT, double newSR, double newMass, double newKappa, double newRestDensity, double newNu);            

    // calculate nearest neighbor indices
    void calc_NNindex( vector<particle> plist );

    // plist    - list of all particles
    void calc_density(vector<particle> plist);

    // calculate restoring pressure
    void calc_pressure();           

    // calculates pressure, viscosity and gravitational force on particle
    void calc_force(vector<particle> plist);

    // updates velocity
    void update_vel();

    // updates positons
    void update_pos();

    // calculates collisions
    void ff_collisions(vector<particle> plist);


  private:
    void fix_windowEdges();


    // smoothing kernal W_ij - where pdiff is the distance between particles 
    double W_gauss(vector<double> rdiff_vec);


    vector<double> Grad_W_gauss(vector<double> rdiff_vec);

    double Laplacian_W_gauss(vector<double> rdiff_vec);            

    // calculates magnitude of 2D vector
    double mag(vector<double> quant);    
    
    // get vector displacement between class and argument particle
    vector<double> calc_rdiff(particle p2);            

    // calc pressure force
    vector<double> F_pressure(vector<particle> plist);

    // calc viscosity force
    vector<double> F_viscosity(vector<particle> plist);
    
    // calc gravitational force
    vector<double> F_gravity();

};


#endif  // PARTICLEMETHODS_H

