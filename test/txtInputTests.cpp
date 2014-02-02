//
//  txtInputTests.cpp
//  nbody
//
//  Created by Annie Jiao on 1/29/14.
//  Copyright (c) 2014 MIT. All rights reserved.
//

#include <gtest/gtest.h>
#include <nbody/Simulation.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <string>

#include <dirent.h>

using namespace std;

// One object at the center with no velocity or acceleration. Should not move.
TEST(txtInputTests, stationary) {
    std::ifstream input{ "resources/nbody/unibody-stationary.txt" };
    nbody::Simulation sim{ input };
    for(int i = 0; i < 40; ++i) {
        sim.saveRun();
        sim.evolveSystem(1e4, .000001);
    }
    sim.saveRun();
    
    ifstream fin("name.txt");
    
    while (fin.is_open()) {

        char line[512];
        fin >> line;
        
        ASSERT_STREQ(line, "1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}

// Six objects move toward center large mass
TEST(txtInputTests, Centering) {
    std::ifstream input{ "resources/nbody/perfectly-symmetric-distribution-move-toward-center.txt" };
    nbody::Simulation sim{ input };
    
    for(int i = 0; i < 40; ++i) {
        sim.saveRun();
        sim.evolveSystem(1e4, .001);       // make sure to get final state
    }
    
    sim.saveRun();
    
    ifstream fin("name.txt");
    
    while (fin.is_open()) {
        
        char line[512];
        fin >> line;
        
        ASSERT_STREQ(line, "7");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+10");
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}


// Six objects converge towards center; seventh object has zero mass.

TEST(txtInputTests, zeroMass) {
    std::ifstream input{ "resources/nbody/centering-distribution-seven-zero-mass.txt" };
    nbody::Simulation sim{ input };
    
    for(int i = 0; i < 40; ++i) {
        sim.saveRun();
        sim.evolveSystem(1e4, .001);       // make sure to get final state
    }
    
    sim.saveRun();
    
    ifstream fin("name.txt");
    
    while (fin.is_open()) {
        
        char line[512];
        fin >> line;
        
        ASSERT_STREQ(line, "7");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        // The seventh object has a weird location/velocity/acceleration, but its mass is zero.
        // Check to see that a zero mass doesn't break the system.
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}



