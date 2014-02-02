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


std::string exec(char* cmd) {
    
    // cd into directory first
    popen("cd", "r");
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}



TEST(txtInputTests, stationary) {
    std::ifstream input{ "resources/nbody/unibody-stationary.txt" };
    nbody::Simulation sim{input};
    for(int i = 0; i < 40; ++i) {
        std::cout << "==EVOLUTION " << i + 1 << "\n";
        sim.saveRun();
        sim.evolveSystem(1e4, .000001);
    }
    sim.saveRun();
    
    string filename = "name";
    ifstream fin("name.txt");
    
    while (fin.is_open()) {
        
        // this is kinda shitty and manual right now
        // read each line and compare answers
        char line[512];
        fin >> line;
        
        ASSERT_STREQ(line, "1");
        
        fin >> line;
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin >> line;
        ASSERT_STREQ(line, "");
        
    }
    
}


