#include <gtest/gtest.h>
#include <stdlib.h>
#include <vector>

#include "particleMethods.h"
#include "windowManager.h"
#include "windowGUI.h"
#include "windowSimulation.h"


// gtest fixture for test


class particleMethodsFixture : public ::testing::Test {
    public:
           
        std::vector<particle> pList;
        
    protected:
        void SetUp() override {
            
            std::vector<double> pos1{1.0, 1.0}, pos2{2.0, 2.0}, pos3{3.0, 3.0};
            std::vector<double> vel1{1.0, 1.0}, vel2{2.0, 2.0}, vel3{3.0, 3.0};   
            
            particle p1(pos1, vel1, 0, 3);
            particle p2(pos2, vel2, 1, 3);
            particle p3(pos3, vel3, 2, 3);
            
            pList = {p1, p2, p3};        
        }

        void TearDown() override {
            // Runs **after each test**
        }
};



class windowManagerFixture : public ::testing::Test {

    
    protected:
        void SetUp() override {
            // Runs **before each test**
        }

        void TearDown() override {
            // Runs **after each test**
        }
};


class windowGUIFixture : public ::testing::Test {

    
    protected:
        void SetUp() override {
            // Runs **before each test**
        }

        void TearDown() override {
            // Runs **after each test**
        }
};


class windowSimulationFixture : public ::testing::Test {

    
    protected:
        void SetUp() override {
            // Runs **before each test**
        }

        void TearDown() override {
            // Runs **after each test**
        }
};


