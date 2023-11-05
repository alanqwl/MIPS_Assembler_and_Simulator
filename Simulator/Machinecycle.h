#ifndef SIMULATOR_H
#define SIMULATOR_H

// System head files
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <set>
#include <cstdio>

#include <fstream>
#include <string>
#include <iostream>
#include <bitset>
#include <malloc.h>
#include <stdio.h>
#include <map>

void simulator(std::string bin_file, std::string codeFile, std::string infile, std::string outfile, std::string checkpoint);
#endif // SIMULATOR_H
