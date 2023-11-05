#include "phase1.h"

bool findTextSegment(std::string line){
    if (line.substr(0, 5) == ".text"){
        return true;
    }else{
        return false;
    }
}

std::string::size_type findCommentIndex(std::string line){
    std::string::size_type idx = line.find("#");
    return idx;
}

bool isLabel(std::string line){
    std::string::size_type idx = line.find(":");
    if (idx != std::string::npos){
        return true;
    }else{
        return false;
    }
}

label findLabel(std::string line, long currentAddress){
    std::string::size_type idx = line.find(":");
    std::string name = line.substr(0, idx);
    label labelInLine;
    labelInLine.address = currentAddress;
    labelInLine.labelName = name;
    return labelInLine;
}
