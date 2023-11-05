#include "labelTable.h"
#include "phase1.h"

std::map<std::string, long> scan1(std::string filename){
    std::ofstream ofile;
    std::ifstream ifile;
    long pc = 0x400000;                 // The variable pc is the program counter.
    std::map <std::string, long> labelTable;
    std::string outputstr = "";
    std::string MIPS_line = "";         // Each line of the MIPS code is store in MIPS_line.
    std::string op = "";
    ifile.open(filename);
    ofile.open("Intermediate.txt");     // Open the output file.

    // Scan the file until the label ".text" is found.
    while (getline(ifile, MIPS_line)){
        if (findTextSegment(MIPS_line)){
            break;
        }
    }
    // Read the MIPS code line by line.
    while (getline(ifile, MIPS_line)){
        if (MIPS_line == ""){                   // It is an empty line.
            continue;
        }
        MIPS_line.erase(0, MIPS_line.find_first_not_of(" "));       // Erase the empty space.
        MIPS_line.erase(0, MIPS_line.find_first_not_of("\t"));
        MIPS_line.erase(0, MIPS_line.find_first_not_of(" "));
        MIPS_line.erase(0, MIPS_line.find_first_not_of("\t"));
        std::string::size_type idx = findCommentIndex(MIPS_line);    // Find where the comment start
        if (idx != std::string::npos){          // There are comments in this line.
            if (idx == 0){
                continue;
            }else{
                if (isLabel(MIPS_line)){        // There is a label in this line.
                    label lab = findLabel(MIPS_line, pc);
                    labelTable[lab.labelName] = lab.address;
                    std::string::size_type codestart = MIPS_line.find_first_not_of(" ", lab.labelName.length() + 1);
                    if (codestart != idx){      // There are codes between the label and the comment.
                        outputstr = MIPS_line.substr(codestart, idx - codestart);
                        ofile << outputstr << std::endl;
                    }else{                      // only label and comment, no codes
                        pc -= 4;                // labels and comments won't be stored in the address.
                    }
                }else{
                    outputstr = MIPS_line.substr(0, idx);
                    ofile << outputstr << std::endl;
                }
            }
        }else{
            // Check whether the output string contains a label.
            if (isLabel(MIPS_line)){
                label lab = findLabel(MIPS_line, pc);
                labelTable[lab.labelName] = lab.address;
                // Check whether there are useful code behind the label.
                std::string::size_type codestart = MIPS_line.find_first_not_of(" ", lab.labelName.length() + 1);
                if (codestart != std::string::npos){
                    int codeLength = MIPS_line.length() - codestart;
                    outputstr = MIPS_line.substr(codestart, codeLength);
                    ofile << outputstr << std::endl;
                }else{
                    pc -= 4;
                }           
            }else{
                outputstr = MIPS_line;
                ofile << outputstr << std::endl;
            }
        }

        pc += 4;
    }
    ifile.close();
    ofile.close();
    return labelTable;
}
