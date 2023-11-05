#include "phase1.h"
#include "labelTable.h"
#include "phase2.h"
#include <iostream>

int main(int argc, char * argv[]){
    if (argc < 4){
        std::cout << "Please enter an input file, an output file, and an expected output file." << std::endl;
        return 0;
    }
    std::map<std::string, long> label;
    std::ifstream output;
    std::ifstream expected_output;
    std::string output_line = "";
    std::string expected_output_line = "";
    std::string inputfile = argv[1];
    std::string expectedOutput = argv[3];
    label = scan1(inputfile);
    scan2(label, argv[2]);
    output.open(argv[2]);
    expected_output.open(expectedOutput);
    while (getline(output, output_line)){
        getline(expected_output, expected_output_line);
        if (expected_output_line.substr(0, 32) != output_line.substr(0, 32)){
            std::cout << "There is something wrong." << std::endl;
            return 0;
        }
    }
    std::cout << " All passed." << std::endl;
    return 0;
}
