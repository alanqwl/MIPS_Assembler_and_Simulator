#ifndef PHASE2_H
#define PHASE2_H

#include <map>
#include <string>
#include <fstream>
#include <bitset>

/*
* The second scan will be conducted in the phase2.cpp. This function 
* scan through the intermediate file. The output will be a txt file of
* the corresponding binary machine code. 
*/
void scan2(std::map<std::string, long> labelTable, std::string filename);
#endif // PHASE2_H
