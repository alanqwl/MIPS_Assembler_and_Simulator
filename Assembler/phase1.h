#ifndef PHASE1_H
#define PHASE1_H

#include <fstream>
#include <string>

/*
* The findTextSegment(std::string line) function:
* This function is used to find out the text segment of the whole file.
*/
bool findTextSegment(std::string line);

/*
* The std::string::size_type findCommentIndex(std::string line) function:
* This function takes each line of code as input and check whether there 
* are comments and where they start.
*/
std::string::size_type findCommentIndex(std::string line);

/*
* The label structure is used to store the information of labels.
*/
struct label{
    std::string labelName;
    int address;
};

/*
* The isLabel(std::string line) function is used to check whether a new
* label is found.
*/
bool isLabel(std::string line);

/*
* The findlabel(std::string line, long) is used to store the information
* of a new label, including its label name and the corresponding address.
*/
label findLabel(std::string line, long);
#endif // PHASE1_H
