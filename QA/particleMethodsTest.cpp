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


