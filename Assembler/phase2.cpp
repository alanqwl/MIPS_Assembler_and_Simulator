#include "phase2.h"

void scan2(std::map<std::string, long> labelTable, std::string filename){
    std::ifstream in;
    std::ofstream out;
    long pc = 0x400000;              // program counter
    std::string codeLine = "";
    std::string output = "";
    std::string op = "";
    std::string funct = "";          // function code for R-type
    std::string sa = "00000";        // Shift amount for R_type
    std::string opcode;
    std::string rs = "";
    std::string rt = "";
    std::string rd = "";
    std::string target = "";
    std::string immediate = "";
    std::string::size_type token;
    std::string::size_type tokenEnd;
    // R-type instructions in the format of {Instruction, Funct}
    std::map<std::string, std::string> R_type_Instruction = {
        {"add", "100000"}, {"addu", "100001"},
        {"and", "100100"}, {"div", "011010"},
        {"divu", "011011"}, {"jalr","001001"},
        {"jr", "001000"}, {"mfhi", "010000"},
        {"mflo", "010010"}, {"mthi", "010001"},
        {"mtlo", "010011"}, {"mult", "011000"},
        {"multu", "011001"}, {"nor", "100111"},
        {"or", "100101"}, {"sll", "000000"},
        {"sllv", "000100"}, {"slt", "101010"},
        {"sltu", "101011"}, {"sra", "000011"},
        {"srav", "000111"}, {"srl", "000010"},
        {"srlv", "000110"}, {"sub", "100010"},
        {"subu", "100011"}, {"syscall", "001100"},
        {"xor", "100110"}
    };
    // I-type instructions in the format of {Instruction, Opcode}
    std::map <std::string, std::string> I_type_Instruction = {
        {"addi", "001000"}, {"addiu", "001001"},
        {"andi", "001100"}, {"beq", "000100"},
        {"bgez", "000001"}, {"bgtz", "000111"},
        {"blez", "000110"}, {"bltz", "000001"},
        {"bne", "000101"}, {"lb", "100000"},
        {"lbu", "100100"}, {"lh", "100001"},
        {"lhu", "100101"}, {"lui", "001111"},
        {"lw", "100011"}, {"ori", "001101"},
        {"sb", "101000"}, {"slti", "001010"},
        {"sltiu", "001011"}, {"sh", "101001"},
        {"sw", "101011"}, {"xori", "001110"},
        {"lwl", "100010"}, {"lwr", "100110"},
        {"swl", "101010"}, {"swr", "101110"}
    };
    // J-type Instructions in the format of {Instruction, Opcode}
    std::map <std::string, std::string> J_type_Instruction = {
        {"j", "000010"}, {"jal", "000011"}
    };
    // Register information
    std::map <std::string, std::string> reg{
        {"$zero", "00000"}, {"$at", "00001"},
        {"$v0", "00010"}, {"$v1", "00011"},
        {"$a0", "00100"}, {"$a1", "00101"},
        {"$a2", "00110"}, {"$a3", "00111"},
        {"$t0", "01000"}, {"$t1", "01001"},
        {"$t2", "01010"}, {"$t3", "01011"},
        {"$t4", "01100"}, {"$t5", "01101"},
        {"$t6", "01110"}, {"$t7", "01111"},
        {"$s0", "10000"}, {"$s1", "10001"},
        {"$s2", "10010"}, {"$s3", "10011"},
        {"$s4", "10100"}, {"$s5", "10101"},
        {"$s6", "10110"}, {"$s7", "10111"},
        {"$t8", "11000"}, {"$t9", "11001"},
        {"$k0", "11010"}, {"$k1", "11011"},
        {"$gp", "11100"}, {"$sp", "11101"},
        {"$fp", "11110"}, {"$ra", "11111"}
    };
    in.open("Intermediate.txt");
    out.open(filename);
    // Read the code line by line.
    while (getline(in, codeLine)) {
        token = codeLine.find_first_of(" ");
        if (token == std::string::npos){
            continue;
        }
        op = codeLine.substr(0, token);
        if (R_type_Instruction.find(op) != R_type_Instruction.end()){       // R-Type Instructions
            opcode = "000000";
            funct = R_type_Instruction[op];
            if (op == "div"
                    || op == "divu"
                    || op == "mult"
                    || op == "multu"){
                rd = "00000";
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rs = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rt = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rt = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "jalr"){
                rt = "00000";
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rd = reg[codeLine.substr(token, tokenEnd)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rs = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rs = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "jr"
                     || op == "mthi"
                     || op == "mtlo"){
                rd = "00000";
                rt = "00000";
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rs = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rs = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "mfhi" || op == "mflo"){
                rt = "00000";
                rd = "00000";
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rd = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rd = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "sll"
                     || op == "sra"
                     || op == "srl"){
                rs = "00000";
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rd = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(",", token);
                rt = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_not_of(" ", tokenEnd);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    int shift = stoi(codeLine.substr(token, tokenEnd - token));
                    std::bitset<5> bin(shift);
                    sa = bin.to_string();
                }else{
                    int shift = stoi(codeLine.substr(token, codeLine.length() - token));
                    std::bitset<5> bin(shift);
                    sa = bin.to_string();
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "sllv"
                     || op == "srav"
                     || op == "srlv"){
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rd = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(",", token);
                rt = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rs = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rs = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else if(op == "syscall"){
                rs = "00000";
                rt = "00000";
                rd = "00000";
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }else{
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rd = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(",", token);
                rs = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    rt = reg[codeLine.substr(token, tokenEnd - token)];
                }else{
                    rt = reg[codeLine.substr(token, codeLine.length() - token)];
                }
                output = opcode + rs + rt + rd + sa + funct;
                out << output << std::endl;
            }
        }else if(op == "j" || op == "jal"){                 // J-Type Instructions(Opcode 00001x)
            if (op == "j"){
                opcode = "000010";
                token = codeLine.find_first_not_of(" ", 1);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    long label_info = labelTable[codeLine.substr(token, tokenEnd - token)];
                    std::bitset<32> target_bin(label_info);
                    target = target_bin.to_string();
                    target = target.substr(4, 26);
                }else{
                    long label_info = labelTable[codeLine.substr(token,codeLine.length() - token)] ;
                    std::bitset<32> target_bin(label_info);
                    target = target_bin.to_string();
                    target = target.substr(4, 26);
                }
                output = opcode + target;
                out << output << std::endl;
            }else{
                opcode = "000011";
                token = codeLine.find_first_not_of(" ", 3);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    long label_info = labelTable[codeLine.substr(token, tokenEnd - token)];
                    std::bitset<32> target_bin(label_info);
                    target = target_bin.to_string();
                    target = target.substr(4, 26);
                }else{
                    long label_info = labelTable[codeLine.substr(token, codeLine.length() - token)];
                    std::bitset<32> target_bin(label_info);
                    target = target_bin.to_string();
                    target = target.substr(4, 26);
                }
                output = opcode + target;
                out << output << std::endl;
            }
        }else{                                              // I-Type Instructions
            opcode = I_type_Instruction[op];
            if (op == "beq" || op == "bne"){
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rs = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(",", token);
                rt = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_not_of(" ", tokenEnd + 1);
                tokenEnd =codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    std::string targetlabel = codeLine.substr(token, tokenEnd - token);
                    long label_info = (labelTable[targetlabel] - pc - 4) / 4;
                    if (label_info < 0){
                        label_info = (-1 * label_info) - 1;
                        std::bitset<16> immediate_bin(label_info);
                        immediate_bin = immediate_bin.flip();
                        immediate = immediate_bin.to_string();
                    }else{
                        std::bitset<16> immediate_bin(label_info);
                        immediate = immediate_bin.to_string();
                    }
                }else{
                    std::string targetlabel = codeLine.substr(token, codeLine.length() - token);
                    long label_info = (labelTable[targetlabel] - pc - 4) / 4;
                    if (label_info < 0){
                        label_info = (-1 * label_info) - 1;
                        std::bitset<16> immediate_bin(label_info);
                        immediate_bin = immediate_bin.flip();
                        immediate = immediate_bin.to_string();
                    }else{
                        std::bitset<16> immediate_bin(label_info);
                        immediate = immediate_bin.to_string();
                    }
                }
                output = opcode + rs + rt + immediate;
                out << output << std::endl;
            }else if(op == "bgez"
                     || op == "bgtz"
                     || op == "blez"
                     || op == "bltz"){
                if (op == "bgez"){
                    rt = "00001";
                }else{
                    rt = "00000";
                }
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rs = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_not_of(" ", tokenEnd + 1);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    std::string targetlabel = codeLine.substr(token, tokenEnd - token);
                    long label_info = (labelTable[targetlabel] - pc - 4) / 4;
                    if (label_info < 0){
                        label_info = (-1 * label_info) - 1;
                        std::bitset<16> immediate_bin(label_info);
                        immediate_bin = immediate_bin.flip();
                        immediate = immediate_bin.to_string();
                    }else{
                        std::bitset<16> immediate_bin(label_info);
                        immediate = immediate_bin.to_string();
                    }
                }else{
                    std::string targetlabel = codeLine.substr(token, codeLine.length() - token);
                    long label_info = (labelTable[targetlabel] - pc - 4) / 4;
                    if (label_info < 0){
                        label_info = (-1 * label_info) - 1;
                        std::bitset<16> immediate_bin(label_info);
                        immediate_bin = immediate_bin.flip();
                        immediate = immediate_bin.to_string();
                    }else{
                        std::bitset<16> immediate_bin(label_info);
                        immediate = immediate_bin.to_string();
                    }
                }
                output = opcode + rs + rt +immediate;
                out << output << std::endl;
            }else if(op == "lb"
                     || op == "lbu"     // rt, immediate(rs)
                     || op == "lh"
                     || op == "lhu"
                     || op == "lui"     // rt, immediate
                     || op == "lw"
                     || op == "sb"
                     || op == "sh"
                     || op == "sw"
                     || op == "lwl"
                     || op == "lwr"
                     || op == "swl"
                     || op == "swr"){
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rt = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_not_of(" ", tokenEnd + 1);
                if (op == "lui"){
                    rs = "00000";
                    tokenEnd = codeLine.find_first_of(" ", token);
                    if (tokenEnd != std::string::npos){
                        std::string immediate_info = codeLine.substr(token, tokenEnd - token);
                        std::bitset<16> immediate_bin(std::stoi(immediate_info));
                        immediate = immediate_bin.to_string();
                    }else{
                        std::string immediate_info = codeLine.substr(token, codeLine.length() - token);
                        std::bitset<16> immediate_bin(std::stoi(immediate_info));
                        immediate = immediate_bin.to_string();
                    }
                }else{
                    tokenEnd = codeLine.find_first_of("(", token);
                    std::string immediate_info = codeLine.substr(token, tokenEnd - token);
                    std::bitset<16> immediate_bin(std::stoi(immediate_info));
                    immediate = immediate_bin.to_string();
                    token = codeLine.find_first_of("$", tokenEnd);
                    tokenEnd = codeLine.find_first_of(")", token);
                    rs = reg[codeLine.substr(token, tokenEnd - token)];
                }
                output = opcode + rs + rt + immediate;
                out << output << std::endl;
            }else{
                token = codeLine.find_first_of("$", token);
                tokenEnd = codeLine.find_first_of(",", token);
                rt = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_of("$", tokenEnd);
                tokenEnd = codeLine.find_first_of(",", token);
                rs = reg[codeLine.substr(token, tokenEnd - token)];
                token = codeLine.find_first_not_of(" ", tokenEnd + 1);
                tokenEnd = codeLine.find_first_of(" ", token);
                if (tokenEnd != std::string::npos){
                    std::string immediate_info = codeLine.substr(token, tokenEnd - token);
                    std::bitset<16> immediate_bin(std::stoi(immediate_info));
                    immediate = immediate_bin.to_string();
                }else{
                    std::string immediate_info = codeLine.substr(token, codeLine.length() - token);
                    std::bitset<16> immediate_bin(std::stoi(immediate_info));
                    immediate = immediate_bin.to_string();
                }
                output = opcode + rs + rt + immediate;
                out << output << std::endl;
            }
        }
        pc += 4;
    }
}
