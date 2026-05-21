#include <gtest/gtest.h>
#include "math.h"

#include "particleMethods.h"
#include "test-fixtures.h"


// test particle simulation parameter update function
TEST_F(particleMethodsFixture, init_conditions)
{
    particle p = pList[0];
    p.update_parameters(0.1, 0.2, 0.3, 0.4, 0.5, 0.6);

    EXPECT_DOUBLE_EQ(p.dt, 0.1);
    EXPECT_DOUBLE_EQ(p.SR, 0.2);
    EXPECT_DOUBLE_EQ(p.mass, 0.3);
    EXPECT_DOUBLE_EQ(p.kappa, 0.4);
    EXPECT_DOUBLE_EQ(p.rest_density, 0.5);
    EXPECT_DOUBLE_EQ(p.nu, 0.6);
}

// test calculating nearest neighbours are in smoothing radius
TEST_F(particleMethodsFixture, NN_singleParticle)
{
    particle p = pList[0];

    p.SR = 5.0;
    p.calc_NNindex(pList);

    EXPECT_EQ(p.neighbor_index[0], 1.0);
    EXPECT_EQ(p.neighbor_index[1], 1.0);
    EXPECT_EQ(p.neighbor_index[2], 1.0);
}


// test calculating nearest neighbours are out of smoothing radius
TEST_F(particleMethodsFixture, NN_multipleParticle)
{
    particle p = pList[0];

    p.SR = 0.01;
    p.calc_NNindex(pList);

    EXPECT_EQ(p.neighbor_index[0],  1.0);   // the particle is in range of itself (check this behaviour)?
    EXPECT_EQ(p.neighbor_index[1], -1.0);
    EXPECT_EQ(p.neighbor_index[2], -1.0);    
}

// density calculartion for only 1 particle in the centre of the smoothing radius 
TEST_F(particleMethodsFixture2, singleParticle_density)
{
    particle p = pList[1];

    p.SR = 1.0;
    p.calc_NNindex(pList);
    p.calc_density(pList);

    //EXPECT_EQ(p.neighbor_index[0], -1.0);   // the particle is in range of itself (check this behaviour)?
    //EXPECT_EQ(p.neighbor_index[1], 1.0);
    //EXPECT_EQ(p.neighbor_index[2], -1.0);    

    // smoothing kernal W_poly6
    double N = 4.0 / ( M_PI * pow(p.SR, 8.0) );
    double outer = pow(p.SR, 6.0); 

    EXPECT_NEAR(p.density,  N * outer, 0.001);
}

// density check for particle on the edge of the smoothing radius (effectively zero influence)
// particel in centre and 2 particlse at edge
TEST_F(particleMethodsFixture2, multiParticle_low_density_influence)
{
    // p1 and p3 are on the smoothing radius edge of p2
    particle p = pList[1];

    p.SR = 1.0;
    p.calc_NNindex(pList);
    p.calc_density(pList);
    
    double N = 4.0 / ( M_PI * pow(p.SR, 8.0) );
    double outer = pow(p.SR, 6.0); 

    EXPECT_NEAR(p.density, N * outer, 0.01);
}

// can add a test for high density - multiple particles 


// creates a negative pressure to move the current density to the higher current (rest) density
TEST_F(particleMethodsFixture, neg_pressure)
{
    // pressure = kappa * ( density - rest_density)

    particle p = pList[1];
    p.SR = 1.0;

    p.kappa = 1.0;
    p.density = 1.0;
    p.rest_density = 3.0;

    p.calc_pressure();

    EXPECT_NEAR(p.pressure, -2.0, 0.01);
}


// creates a positive pressure to move the current density to the lower current (rest) density
TEST_F(particleMethodsFixture, pos_pressure)
{
    // pressure = kappa * ( density - rest_density)

    particle p = pList[1];
    p.SR = 1.0;

    p.kappa = 1.0;
    p.density = 8.0;
    p.rest_density = 3.0;

    p.calc_pressure();

    EXPECT_NEAR(p.pressure, 5.0, 0.01);
}



// testing arithmetic for velocity update
TEST_F(particleMethodsFixture, velocity_euler)
{
    //velocity[0] = (dt * force[0]) / mass; 
    
    particle p = pList[1];
    
    p.dt = 2.0;
    p.force[0] = 4.0;
    p.mass = 2.0;
    // ( 2.0 x 2.0 ) / 4.0

    p.update_vel();

    EXPECT_NEAR(p.velocity[0], 4.0, 0.01);
}


// testing arithmetic for velocity update with zero mass ( should correct to mass=1 )
TEST_F(particleMethodsFixture, velocity_euler_nonzero_mass)
{
    //velocity[0] = (dt * force[0]) / mass; 
    
    particle p = pList[1];
    
    p.dt = 1.0;
    p.force[1] = 10.0;
    p.mass = 1.0;
    
    // ( 1.0 x 10.0 ) / 0.0
    // should correct to :
    // ( 1.0 x 10.0 ) / 1.0

    p.update_vel();

    EXPECT_NEAR(p.velocity[1], 10.0, 0.01);
}


// testing window binds (100,100)
TEST_F(particleMethodsFixture, pos_window_edge_binds)
{
    // dx_dt = position[0] + ( dt * velocity[0] );
    
    particle p = pList[1];
    
    p.position[0] = 60.0;
    p.position[1] = 50.0;
    
    p.dt = 5.0;
    
    p.velocity[0] = 20.0; 
    p.velocity[1] = 10.0;


    p.update_pos();
    
    // 60.0 + ( 5.0 x 20.0 ) = 160
    // 50.0 + ( 10.0 x 10.0 ) = 100

    // this should bind max positions to (98.0,98.0)
    p.fix_windowEdges();   

    EXPECT_NEAR(p.position[0], 98.0, 0.01);
    EXPECT_NEAR(p.position[1], 98.0, 0.01);
}


// testing window binds (100,100)
TEST_F(particleMethodsFixture, neg_window_edge_binds)
{
    // dx_dt = position[0] + ( dt * velocity[0] );
    
    particle p = pList[1];
    
    p.position[0] = -50.0;
    p.position[1] = -20.0;
    
    p.dt = 5.0;
    
    p.velocity[0] = -40.0; 
    p.velocity[1] = -20.0;


    p.update_pos();
    
    // -50.0 + ( 5.0 x -40.0 ) = -250
    // -20.0 + ( 10.0 x -20.0 ) = -220

    // this should bind max positions to (-98.0,-98.0)
    p.fix_windowEdges();   

    EXPECT_NEAR(p.position[0], -98.0, 0.01);
    EXPECT_NEAR(p.position[1], -98.0, 0.01);
}
