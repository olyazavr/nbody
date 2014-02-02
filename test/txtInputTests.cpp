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
    
    // read the result
    ifstream fin("name.txt");       // all the outputs will have the filename "name.txt"
                                    // new outputs get overwritten
    
    while (fin.is_open()) {

        char line[512];     // give enough character allocation for one line
        fin >> line;        // read in a line
        
        ASSERT_STREQ(line, "1");        // check to see line is as expected
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "");         // all test files and outputs have a hanging linebreak at the end
        
    }
    
}

// Six objects move toward center large mass
// ideal situation

TEST(txtInputTests, Centering) {
    std::ifstream input{ "resources/nbody/perfectly-symmetric-distribution-move-toward-center.txt" };
    nbody::Simulation sim{ input };
    
    /*
     7
     1 0 0  0 0 0  0 0 0 2.0e+1
     0 1 0  0 0 0  0 0 0 2.0e+1
     0 0 1  0 0 0  0 0 0 2.0e+1
     0 0 -1  0 0 0  0 0 0 2.0e+1
     0 -1 0  0 0 0  0 0 0 2.0e+1
     -1 0 0  0 0 0  0 0 0 2.0e+1
     0 0 0  0 0 0  0 0 0 1.0e+10

     */
    
    for(int i = 0; i < 40; ++i) {
        sim.saveRun();
        sim.evolveSystem(1e4, .001);       // make sure to take enough steps to get final state
    }
    
    sim.saveRun();
    
    ifstream fin("name.txt");
    
    while (fin.is_open()) {
        
        char line[512];
        fin >> line;
        
        ASSERT_STREQ(line, "7");       // output should still have seven bodies
        
        // all of them should move towards the origin; mass is converved
        
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
// Test to see that zero mass doesn't break the system

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
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}

// BINARY SIMPLE
// Tests the file provided

TEST(txtInputTests, binarySimple) {
    std::ifstream input{ "resources/nbody/binary-system-simple.txt" };
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
        
        ASSERT_STREQ(line, "2");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 1.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}

// Approximately symmetric & stable objects
// Six objects move toward center large mass

TEST(txtInputTests, centeringApprox) {
    std::ifstream input{ "resources/nbody/approx-equal-centralize.txt" };
    
    /*
     1.02342 0 -0.0023123  0 0 0  0 0.00078907896 0 2.05123124e+1
     0 1.132123 0.0187807  0 0.0004351234 0  0 0 0 2.0356345723e+1
     -0.00806789 0 1.348568  0 0 0  0 0 -0.0045235 2.067678956e+1
     0.003452 -0.008 -1.85985678  0 -0.0003452435 0  0 0 0 2.01234565e+1
     0.00786780 -1.45674567 0.01312  0.0002131234 0 0  0.0003541235 0 0 2.03658578e+1
     -1.6785634 -0.01234123 0.03456123  0.001243123 0 0  0 -0.00131234 0 2.0356657e+1
     0.0005678456 -0.000025236 0.00004657345  0 0.00345123 0  0.01123452346 0 0 2.034256345e+10

     */
    
    
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
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.05123124e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0356345723e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.067678956e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.01234565e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.03658578e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0356657e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.034256345e+10");
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}

// Shift a converge by +10.5, +10.5, +10.5, make sure it's not just showing 0 0 0 every time

TEST(txtInputTests, convergeShifted) {
    
    /*
     
     7
     11.5 10.5 10.5  0 0 0  0 0 0 2.0e+1
     10.5 11.5 10.5  0 0 0  0 0 0 2.0e+1
     10.5 10.5 11.5  0 0 0  0 0 0 2.0e+1
     10.5 10.5 9.5  0 0 0  0 0 0 2.0e+1
     10.5 9.5 10.5  0 0 0  0 0 0 2.0e+1
     9.5 10.5 10.5  0 0 0  0 0 0 2.0e+1
     10.5 10.5 10.5  0 0 0  0 0 0 1.0e+10

     */
    
    std::ifstream input{ "resources/nbody/converge-shifted.txt" };
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
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "10.5 10.5 10.5  0 0 0  0 0 0 1.0e+10");
        
        fin >> line;
        ASSERT_STREQ(line, "");
    }
    
}




