#include <stdlib.h>
#include <bits/stdc++.h>
#include <vector>

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
    particle(){};

    // constructor
    particle( vector<double> pos, vector<double> vel, int index, int nParticles) 
            : position(pos), velocity(vel), index_(index), nParticles_(nParticles){
        
        // check arguments valid
        if (2 != pos.size()) {
            printf("invalid initial position\n");
        }

        if (2 != vel.size()) {
            printf("invalid initial velocity\n");
        }

        density = 0.0;
        neighbor_index = (int*)malloc( nParticles_ * sizeof(int) );
    
    }

    
    static void update_parameters(double newDT, double newSR, double newMass, double newKappa, double newRestDensity, double newNu){
        
        dt = newDT;
        SR = newSR;
        mass = newMass;
        kappa = newKappa;
        rest_density = newRestDensity;
        nu = newNu;
    }

    // calculate nearest neighbor indices
    void calc_NNindex( vector<particle> plist ){
       
        for (int i=0; i<nParticles_; i++){
            neighbor_index[i] = -1;
        }

        for (particle &p : plist){
            
            // calc vector & magnitude displacement
            vector<double> rdiff_vec = calc_rdiff(p);
            double rdiff = mag(rdiff_vec);
            
            // sets cloests neighbor particles
            if (rdiff <= SR){
                neighbor_index[p.index_] = 1;
            } else {
                neighbor_index[p.index_] = -1;
            
            } 
        }

        // checking NN
        //printf("p%d | NN: [%d,%d,%d,%d]\n", index_,
        //                                    neighbor_index[0],
        //                                    neighbor_index[1],
        //                                    neighbor_index[2],
        //                                    neighbor_index[3] );


    }

    // plist    - list of all particles
    void calc_density(vector<particle> plist){
        
        density = 0.0;
        // loop over all particles
        for (particle &p : plist){

            // potential neighbor index
            int InRange = neighbor_index[p.index_];

            // check if neighbor particle is in smoothin radius
            if (1 == InRange){
                    
                vector<double> rdiff_vec = calc_rdiff(p);
                    
                // quadratic smoothing kernal
                //double r_mag = mag(rdiff_vec);
                //density += mass * W_quad(r_mag);

                // guassian smoothing kernal
                density += mass * W_gauss(rdiff_vec);

                //printf("particle: %d | W_gauss: %f\n", p.index_, W_gauss(rdiff_vec) );
                //printf("W_quad: %f\n", W_quad(r_mag) );

            
            }
        }    
    

        // check density for each particle
        //printf("p%d | density %f\n", index_, density);
     }

    void calc_pressure(){
   
           pressure = kappa * ( density - rest_density ); 
    }


    // calculates pressure, viscosity and gravitational force on particle
    void calc_force(vector<particle> plist){
   

        force[0] = 0.0;
        force[1] = 0.0;

        // pressure + viscocity force is bugged and always zero

        vector<double> Fp = F_pressure(plist);
        vector<double> Fv = F_viscosity(plist);
        vector<double> Fg = F_gravity();


        force[0] = Fp[0]; // /*+ Fv[0]*/ + Fg[0];    
        force[1] = Fp[1]; // /*+ Fv[1]*/ + Fg[1];    

        // checking forces
        //printf("p%d | force[0]: %f | force[1]: %f \n", index_, Fv[0], Fv[1]);        
    }

    // updates velocity
    void update_vel(){

        velocity[0] = (dt * force[0]) / mass; 
        velocity[1] = (dt * force[1]) / mass; 
       
    }

    // updates positons
    void update_pos(){
    
        double dx_dt = position[0] + ( dt * velocity[0] );
        double dy_dt = position[1] + ( dt * velocity[1] );

        position[0] = dx_dt;        
        position[1] = dy_dt;        
 
        // fix window boundry
        fix_windowEdges();   
    }



    void ff_collisions(vector<particle> plist) {
        
        for (particle &p : plist) {

            // if particles are colliding
            if (position[0] == p.position[0] && position[1] == p.position[1]) {
                velocity[0] *= -0.75;
                velocity[1] *= -0.75;
            }

        }
    
    }


  private:
    void fix_windowEdges() {

        double x_pos = position[0];
        double y_pos = position[1];

        // fix x-axis particle to inside window
        if (x_pos >= 98.0 ) {
            position[0] = 98.0;
            //velocity[0] *= -0.8;
        } else if (x_pos <= -98.0) {
            position[0] = -98.0;
            //velocity[0] *= -0.8;
        }
        

        // fix y-axis particle to inside window
        if (y_pos >= 98.0) {
            position[1] = 98.0;
            //velocity[1] *= -0.8;
        } else if (y_pos <= -98.0) {
            position[1] = -98.0;
            //velocity[1] *= -0.8;
        }
    }

    // smoothing kernal W_ij - where pdiff is the distance between particles 
    double W_gauss(vector<double> rdiff_vec){
        
        // normalisation
        //double N = 1.0 / ( 2.0 * pow(M_PI, 1.5) * abs(SR) );
        double N = 1.0 / ( M_PI * SR * (1.0 - exp(-1.0)) );

        // r^2
        double r_dot = rdiff_vec[0] * rdiff_vec[0] + rdiff_vec[1] * rdiff_vec[1];
        //double rmag = mag(rdiff_vec);

        // gaussian exponential
        double exponent = -1.0 * r_dot / pow(SR, 2.0);
        double e =  exp( exponent );

        return N * e;    
    }

    vector<double> Grad_W_gauss(vector<double> rdiff_vec){
    
        double Ax = ( -2.0 * rdiff_vec[0] ) / pow(SR, 2.0);
        double Ay = ( -2.0 * rdiff_vec[1] ) / pow(SR, 2.0);

        double grad_Wx = W_gauss(rdiff_vec) * Ax;
        double grad_Wy = W_gauss(rdiff_vec) * Ay;
        
        //printf("rdiffx: %f\n", rdiff_vec[0]);
        
        vector<double> grad_W = {grad_Wx, grad_Wy};

        return grad_W;
    }

    double Laplacian_W_gauss(vector<double> rdiff_vec){
        
        // r^2
        double r_dot = rdiff_vec[0] * rdiff_vec[0] + rdiff_vec[1] * rdiff_vec[1];
        

        double A  = ( (4.0 * r_dot) / pow(SR, 4.0) ) - ( 2.0 / pow(SR, 2.0) );  // there might be another d multiplied by 2.0

        return A * W_gauss(rdiff_vec);

    }

    // calculates magnitude of 2D vector
    double mag(vector<double> quant){

        double mag = quant[0] * quant[0] + quant[1] * quant[1];
        return sqrt(mag);
    }
    
    
    // get vector displacement between class and argument particle
    vector<double> calc_rdiff(particle p2){
        
        double x_coor = position[0] - p2.position[0];
        double y_coor = position[1] - p2.position[1];
        
        vector<double> rdiff = {x_coor, y_coor};
        return rdiff;
    }

    // calc pressure force
    vector<double> F_pressure(vector<particle> plist){

        double Grad_px = 0.0;
        double Grad_py = 0.0;

        for (particle p : plist){
        
            int InRange = neighbor_index[p.index_];

            // check if neighbor particle is in smoothin radius & skipping itself 
            if ( 1 == InRange && index_ != p.index_ ) {

                // calculating grad_Pressure
                double A = p.mass * ( (pressure + p.pressure) / (2.0 * p.density) );    

                vector<double> rdiff_vec = calc_rdiff(p);
                vector<double> Grad_W = Grad_W_gauss(rdiff_vec);
            
                Grad_px += (A * Grad_W[0]);
                Grad_py += (A * Grad_W[1]);
                //printf("p.index: %d, p.pos: %f | index: %d, pos %f \n", p.index_, p.position[0], index_, position[0]);
            }
        }
        //printf("mass: %f | density: %f | del_px: %f \n", mass, density, del_px);

        double Fx_p =  -1.0 * (mass/density) * Grad_px;
        double Fy_p =  -1.0 * (mass/density) * Grad_py;
        
        vector<double> Fp = {Fx_p, Fy_p};
        return Fp;
    }


    // calc viscosity force
    vector<double> F_viscosity(vector<particle> plist){
    
        double laplacian_vx = 0.0;
        double laplacian_vy = 0.0;

        for (particle p : plist){
        
            int InRange = neighbor_index[p.index_];

            // check if neighbor particle is in smoothin radius & skipping itself
            if (1 == InRange && index_ != p.index_) {

                // calculating constant
                double vx_diff =  p.velocity[0] - velocity[0];    
                double vy_diff =  p.velocity[1] - velocity[1];

                //double px_diff = position[0] - p.position[0];
                //double py_diff = position[1] - p.position[1];
                
                vector<double> rdiff_vec = calc_rdiff(p);
                double Laplacian_W = Laplacian_W_gauss(rdiff_vec);

                //double pdiff_dot = px_diff * px_diff + py_diff * py_diff;
                //double pdiff_dot_W = px_diff * Grad_W[0] + py_diff * Grad_W[1];

                
                double Ax = ( p.mass / p.density ) * vx_diff;    
                double Ay = ( p.mass / p.density ) * vy_diff;    

            
                laplacian_vx += Ax * Laplacian_W;
                laplacian_vy += Ay * Laplacian_W;
            }
        }

        double Fx_v = mass * nu * 2 * laplacian_vx;
        double Fy_v = mass * nu * 2 * laplacian_vy;
        
        vector<double> Fv = {Fx_v, Fy_v};
        return Fv;
    }
    
    // calc gravitational force
    vector<double> F_gravity(){

        double Fy_g = -1.0 * mass * 3.8;
        vector<double> Fg = {0.0, Fy_g};
        
        return Fg;
    }




};




