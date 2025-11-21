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

TEST_F(particleMethodsFixture, smallDensity)
{
    particle p = pList[0];

    p.SR = 0.5;
    p.calc_NNindex(pList);
    p.calc_density(pList);

    EXPECT_NEAR(p.density,  1.0, 0.01);
}


TEST_F(particleMethodsFixture, largeDensity)
{
    // p3 is a full SR away from p1 so close to no influence from smoothing func
    // p2 is half SR away from p1 so half contribution from smoothing func
    particle p = pList[0];

    p.SR = 3.0;
    p.calc_NNindex(pList);
    p.calc_density(pList);

    EXPECT_NEAR(p.density,  1.5, 0.01);

}
