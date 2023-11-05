#ifndef LABELTABLE_H
#define LABELTABLE_H

#include <map>
#include <string>
#include <fstream>

/*
* The first scan of the file is performed in the labelTable.cpp
* This function takes the original MIPS code file as input and 
* output an intermediate file with no comments, no labels in each
* line. Moreover, the blankspace or tab will be discarded
*/
std::map<std::string, long> scan1(std::string filename);

#endif // LABELTABLE_H
