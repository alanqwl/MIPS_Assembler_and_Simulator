#include "Machinecycle.h"

int main(int argc, char* argv[]){
    if(argc != 6){
        std::cout << "Please enter again." << std::endl;
        return 0;
    }
    std::string inputbinfile = argv[2];
    std::string inputsrcfile = argv[1];
    std::string input = argv[4];
    std::string output = argv[5];
    std::string checkpointfile = argv[3];
    simulator(inputbinfile, inputsrcfile, input, output, checkpointfile);
    return 0;
}
