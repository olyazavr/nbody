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
#include <stdio.h>

#include <dirent.h>

int MAX_CHARS_PER_LINE = 512;


 // if dirent ends up not working out this is how to commandline bullshit
 // probably shouldn't use ever, muy bad
 
 
 
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
        
        /* 1*10^10 steps, each dt constant size */
        
        sim.evolveSystem(1e10, 1);
    }
    sim.saveRun();
    
    //auto result = exec("ls");     use ls to get filename
    
    DIR *dir;
    struct dirent *entry;
    
    if ((dir = opendir ("output path")) != NULL) {
        //still need output path
        
        //get dat file
        
        std::string filename = entry->d_name;
        closedir (dir);
        
    } else {
        
        // well this sucks
        perror ("error reading output file");
        return EXIT_FAILURE;
    }
     
    
    
    std::ifstream fin;
    fin.open("name.txt");
    
    while (!fin.eof()) {
        
        // this is kinda shitty and manual right now
        // read each line and compare answers
        
        char line[MAX_CHARS_PER_LINE];
        fin.getline(line, MAX_CHARS_PER_LINE);
        ASSERT_STREQ(line, "1");
        
        fin.getline(line, MAX_CHARS_PER_LINE);
        ASSERT_STREQ(line, "0 0 0  0 0 0  0 0 0 2.0e+1");
        
        fin.getline(line, MAX_CHARS_PER_LINE);
        ASSERT_STREQ(line, "");
        
    }
    /* delete so the output directory only has one entry */
    remove(filename + ".txt");
    
    
}


