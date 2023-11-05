#include "Machinecycle.h"
#include "R_type.h"
#include "I_type.h"
#include "J_type.h"
#include "Dump.h"


void simulator(std::string bin_file, std::string codeFile, std::string infile, std::string outfile, std::string checkpoint){
    std::ifstream bin_in;               // read the binary machine code file
    std::ifstream data_in;              // read the static data in the source code file
    std::ifstream input;                // Corresponds to the I/O syscall's input.
    std::ofstream output;               // Corresponds to the I/O syscall's output.
    std::string bin_instruction;
    std::string data_info;
    int int_instruction;
    int simulate_address;
    void* textSegmentStart = malloc(6291456);
    void* move = textSegmentStart;
    void* staticDataStart = (char*)textSegmentStart + 1048576;

//    memory allocation for text segment
    bin_in.open(bin_file);
    simulate_address = 0x400000;
    while (getline(bin_in, bin_instruction)){
        int_instruction = stol(bin_instruction, 0, 2);
        *(int*)move = int_instruction;
        move = (int*)move + 1;
        simulate_address += 4;
    }
    bin_in.close();

    move = staticDataStart;
    data_in.open(codeFile);
    while (getline(data_in, data_info)){
        if (data_info.substr(0, 5) == ".data"){
            break;
        }
    }

//    Memory allocation for static data section.
    simulate_address = 0x500000;
    while (getline(data_in, data_info)){
        data_info.erase(0, data_info.find_first_not_of(" "));
        data_info.erase(0, data_info.find_first_not_of("\t"));
        data_info.erase(0, data_info.find_first_not_of(" "));
        data_info.erase(0, data_info.find_first_not_of("\t"));
        if (data_info.substr(0, 5) == ".text") break;
        // Check whether the line is an empty line or comment line.
        if (data_info == "" || data_info[0] == '#'){
            continue;
        }
        std::string::size_type idx1 = data_info.find_first_of(".");
        std::string::size_type idx2 = data_info.find_first_of(" ", idx1);
        std::string data_type = data_info.substr(idx1, idx2 - idx1);
        if (data_type == ".ascii" || data_type == ".asciiz"){
            idx1 = data_info.find_first_of("\"");
            idx2 = data_info.find_first_of("\"", idx1 + 1);
            std::string data = data_info.substr(idx1 + 1, idx2 - idx1 - 1);
            for (unsigned int i = 0; i < data.length(); i++){
                char* value = (char*)move;
                // Check whether there is a '\n' or '\0'.
                if (data[i] == '\\'){
                    switch (data[i + 1]){
                        case 'n':
                            *value = '\n';
                            i += 1;
                            break;
                        case '0':
                            *value = '\0';
                            i += 1;
                            break;
                        default:
                            *value = data[i];
                            break;
                    }
                }else{
                    *value = data[i];
                }
                move = (char*)move + 1;
                simulate_address += 1;
            }
            // Add a '\0' behind the string.
            if (data_type == ".asciiz"){
                char* value = (char*)move;
                *value = '\0';
                move = (char*)move + 1;
                simulate_address += 1;
            }
            // Move to the next starting address.
            if (simulate_address % 4 != 0){
                move = (char*)move + (4 - (simulate_address % 4));
                simulate_address = simulate_address + (4 - (simulate_address % 4));
            }
        }else if (data_type == ".word" || data_type == ".half"){
            idx1 = data_info.find_first_not_of(" ", idx2 + 1);
            idx2 = data_info.find_first_of(",", idx1);
            while(idx2 != std::string::npos){
                if (data_type == ".word"){
                    int data = stoi(data_info.substr(idx1, idx2 - idx1));
                    int* value = (int*)move;
                    *value = data;
                    move = (int*)move + 1;
                    simulate_address += 4;
                }else{                      // The data type is half.
                    short data = stoi(data_info.substr(idx1, idx2 - idx1));
                    short* value = (short*)move;
                    *value = data;
                    move = (short*)move + 1;
                    simulate_address += 2;
                }
                idx1 = data_info.find_first_not_of(" ", idx2 + 1);
                idx2 = data_info.find_first_of(",", idx1);
            }
            idx2 = data_info.find_first_of(" ", idx1);
            int last_data;                  // manipulate the last data
            if (idx2 != std::string::npos){
                last_data = stoi(data_info.substr(idx1, idx2 - idx1));
            }else{
                last_data = stoi(data_info.substr(idx1, data_info.length() - idx1));
            }
            if (data_type == ".word"){
                int* value = (int*)move;
                *value = last_data;
                move = (int*)move + 1;
                simulate_address += 4;
            }else{                          // half word data type
                short* value = (short*)move;
                *value = (short)last_data;
                move = (short*)move + 1;
                simulate_address += 2;
            }
            // address alignment
            if (simulate_address % 4 != 0){
                move = (char*)move + (4 - (simulate_address % 4));
                simulate_address = simulate_address + (4 - (simulate_address % 4));
            }
        }else{                              // .byte data type
            idx1 = data_info.find_first_not_of(" ", idx2 + 1);
            idx2 = data_info.find_first_of(",", idx1);
            while(idx2 != std::string::npos){
                int data = stoi(data_info.substr(idx1, idx2 - idx1));
                char* value = (char*)move;
                *value = data;
                move = (char*)move + 1;
                simulate_address += 1;
                idx1 = data_info.find_first_not_of(" ", idx2 + 1);
                idx2 = data_info.find_first_of(",", idx1);
            }
            idx2 = data_info.find_first_of(" ", idx1);
            int last_data;
            if (idx2 != std::string::npos){
                last_data = stoi(data_info.substr(idx1, idx2 - idx1));
            }else{
                last_data = stoi(data_info.substr(idx1, data_info.length() - idx1));
            }
            char* value = (char*) move;
            *value = last_data;
            move = (char*) move + 1;
            simulate_address += 1;
            if (simulate_address % 4 != 0){
                move = (char*)move + (4 - (simulate_address % 4));
                simulate_address = simulate_address + (4 - (simulate_address % 4));
            }
        }
    }
    data_in.close();
    void* dynamicData_ptr = move;               // The dynamic data segment start here.

//    Basic register allocation
    void* reg = malloc(140);
    int* zero = (int*)reg;
    int* at = (int*)reg + 1;  
    int* v0 = (int*)reg + 2;
    int* v1 = (int*)reg + 3;
    int* a0 = (int*)reg + 4;
    int* a1 = (int*)reg + 5;
    int* a2 = (int*)reg + 6;
    int* a3 = (int*)reg + 7;
    int* t0 = (int*)reg + 8;
    int* t1 = (int*)reg + 9;
    int* t2 = (int*)reg + 10;
    int* t3 = (int*)reg + 11;
    int* t4 = (int*)reg + 12;
    int* t5 = (int*)reg + 13;
    int* t6 = (int*)reg + 14;
    int* t7 = (int*)reg + 15;
    int* s0 = (int*)reg + 16;
    int* s1 = (int*)reg + 17;
    int* s2 = (int*)reg + 18;
    int* s3 = (int*)reg + 19;
    int* s4 = (int*)reg + 20;
    int* s5 = (int*)reg + 21;
    int* s6 = (int*)reg + 22;
    int* s7 = (int*)reg + 23;
    int* t8 = (int*)reg + 24;
    int* t9 = (int*)reg + 25;
    int* k0 = (int*)reg + 26;
    int* k1 = (int*)reg + 27;
    int* gp = (int*)reg + 28;
    int* sp = (int*)reg + 29;
    int* fp = (int*)reg + 30;
    int* ra = (int*)reg + 31;
    int* PC = (int*)reg + 32;
    int* HI = (int*)reg + 33;
    int* LO = (int*)reg + 34;
    *at = 0;
    *zero = 0;
    *v0 = 0;
    *v1 = 0;
    *a0 = 0;
    *a1 = 0;
    *a2 = 0;
    *a3 = 0;
    *t0 = 0;
    *t1 = 0;
    *t2 = 0;
    *t3 = 0;
    *t4 = 0;
    *t5 = 0;
    *t6 = 0;
    *t7 = 0;
    *s0 = 0;
    *s1 = 0;
    *s2 = 0;
    *s3 = 0;
    *s4 = 0;
    *s5 = 0;
    *s6 = 0;
    *s7 = 0;
    *t8 = 0;
    *t9 = 0;
    *k0 = 0;
    *k1 = 0;
    *ra = 0;
    *HI = 0;
    *LO = 0;
    *PC = 0x400000;
    *gp = 0x508000;
    *sp = 0xA00000;     // The stack pointer points to the last word in the executing procedure's stack frame.
    *fp = 0xA00000;     // The frame pointer points to the first word in the executing procedure's stack frame.
    std::map<std::string, int*> reg_map = {
        {"00000", zero}, {"00001", at},
        {"00010", v0}, {"00011", v1},
        {"00100", a0}, {"00101", a1},
        {"00110", a2}, {"00111", a3},
        {"01000", t0}, {"01001", t1},
        {"01010", t2}, {"01011", t3},
        {"01100", t4}, {"01101", t5},
        {"01110", t6}, {"01111", t7},
        {"10000", s0}, {"10001", s1},
        {"10010", s2}, {"10011", s3},
        {"10100", s4}, {"10101", s5},
        {"10110", s6}, {"10111", s7},
        {"11000", t8}, {"11001", t9},
        {"11010", k0}, {"11011", k1},
        {"11100", gp}, {"11101", sp},
        {"11110", fp}, {"11111", ra}
    };

//    The following map will be used for recognition
//    {function code, operation}
    std::map<std::string, std::string> R_type = {
        {"100000", "add"}, {"100001", "addu"},
        {"100100", "and"}, {"011010", "div"},
        {"011011", "divu"}, {"001001", "jalr"},
        {"001000", "jr"}, {"010000", "mfhi"},
        {"010010", "mflo"}, {"010001", "mthi"},
        {"010011", "mtlo"}, {"011000", "mult"},
        {"011001", "multu"}, {"100111", "nor"},
        {"100101", "or"}, {"000000", "sll"},
        {"000100", "sllv"}, {"101010", "slt"},
        {"101011", "sltu"}, {"000011", "sra"},
        {"000111", "srav"}, {"000010", "srl"},
        {"000110", "srlv"}, {"100010", "sub"},
        {"100011", "subu"}, {"001100", "syscall"},
        {"100110", "xor"}
    };
//    {operation code, operation}
    std::map<std::string, std::string> I_type = {
        {"001000", "addi"}, {"001001", "addiu"},
        {"001100", "andi"}, {"000100", "beq"},
        {"000001", "bgez"}, {"000111", "bgtz"},
        {"000110", "blez"}, {"000001", "bltz"},
        {"000101", "bne"}, {"100000", "lb"},
        {"100100", "lbu"}, {"100001", "lh"},
        {"100101", "lhu"}, {"001111", "lui"},
        {"100011", "lw"}, {"001101", "ori"},
        {"101000", "sb"}, {"001010", "slti"},
        {"001011", "sltiu"}, {"101001", "sh"},
        {"101011", "sw"}, {"001110", "xori"},
        {"100010", "lwl"}, {"100110", "lwr"},
        {"101010", "swl"}, {"101110", "swr"}
    };
//    {operation code, operation}
    std::map<std::string, std::string> J_type = {
        {"000010", "j"}, {"000011", "jal"}
    };

    // checkpoint initilization
    std::set<int> checkpoints;
    FILE *fr = fopen(checkpoint.c_str(), "r");
    int tmp, i = 0;
    while(fscanf(fr, "%d", &tmp) != EOF){
        checkpoints.insert(tmp);
    }
    fclose(fr);


    int* exe = (int*)textSegmentStart;      // A pointer that locate the exact address of the machine code.
    bool exec = true;                       // A boolean variable to show when the program terminates
    int ins_count = 0;                      // Count the number of executions, used for dumping file.
    input.open(infile);                     // Open the input file used for syscall.
    output.open(outfile);                   // Open the output file.
    while (exec){
        checkpoint_memory(ins_count, checkpoints, textSegmentStart);
        if(checkpoints.count(ins_count)){
            std::string name = "register_" + std::to_string(ins_count) + ".bin";
            FILE* reg_file = fopen(name.c_str(), "wb");
            // Register dump
            fwrite(zero, 4, 1, reg_file);
            fwrite(at, 4, 1, reg_file);
            fwrite(v0, 4, 1, reg_file);
            fwrite(v1, 4, 1, reg_file);
            fwrite(a0, 4, 1, reg_file);
            fwrite(a1, 4, 1, reg_file);
            fwrite(a2, 4, 1, reg_file);
            fwrite(a3, 4, 1, reg_file);
            fwrite(t0, 4, 1, reg_file);
            fwrite(t1, 4, 1, reg_file);
            fwrite(t2, 4, 1, reg_file);
            fwrite(t3, 4, 1, reg_file);
            fwrite(t4, 4, 1, reg_file);
            fwrite(t5, 4, 1, reg_file);
            fwrite(t6, 4, 1, reg_file);
            fwrite(t7, 4, 1, reg_file);
            fwrite(s0, 4, 1, reg_file);
            fwrite(s1, 4, 1, reg_file);
            fwrite(s2, 4, 1, reg_file);
            fwrite(s3, 4, 1, reg_file);
            fwrite(s4, 4, 1, reg_file);
            fwrite(s5, 4, 1, reg_file);
            fwrite(s6, 4, 1, reg_file);
            fwrite(s7, 4, 1, reg_file);
            fwrite(t8, 4, 1, reg_file);
            fwrite(t9, 4, 1, reg_file);
            fwrite(k0, 4, 1, reg_file);
            fwrite(k1, 4, 1, reg_file);
            fwrite(gp, 4, 1, reg_file);
            fwrite(sp, 4, 1, reg_file);
            fwrite(fp, 4, 1, reg_file);
            fwrite(ra, 4, 1, reg_file);

            fwrite(PC, 4, 1, reg_file);
            fwrite(HI, 4, 1, reg_file);
            fwrite(LO, 4, 1, reg_file);

            fclose(reg_file);
        }
        std::bitset<32> bst_ins(*exe);
        std::string ins = bst_ins.to_string();                  
        std::string funct;                                      // operation name, such as "add", "addi", etc.
        std::string opcode = ins.substr(0, 6);                  // operation code
        if(opcode == "000000"){                 // R-type instruction goes here.
            funct = R_type[ins.substr(26, 6)];
            int* rs = reg_map[ins.substr(6, 5)];
            int* rt = reg_map[ins.substr(11, 5)];
            int* rd = reg_map[ins.substr(16, 5)];
            std::string sa = ins.substr(21, 5);
            if(funct == "add" || funct == "addu"){
                add_and_addu(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "sub" || funct == "subu"){
                sub_and_subu(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "mult" || funct == "multu"){
                mult_and_multu(rs, rt, HI, LO);
                *PC = *PC + 4;
            }
            else if(funct == "div" || funct == "divu"){
                div_and_divu(rs, rt, HI, LO);
                *PC = *PC + 4;
            }
            else if(funct == "jal"){
                jal(rd, rs, PC);
            }
            else if(funct == "jr"){
                jr(rs, PC);
            }
            else if(funct == "mfhi"){
                mfhi(rd, HI);
                *PC = *PC + 4;
            }
            else if(funct == "mflo"){
                mflo(rd, LO);
                *PC = *PC + 4;
            }
            else if(funct == "mthi"){
                mthi(rs, HI);
                *PC = *PC + 4;
            }
            else if(funct == "mtlo"){
                mtlo(rs, LO);
                *PC = *PC + 4;
            }
            else if(funct == "xor"){
                xor_ins(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "nor"){
                nor_ins(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "or"){
                or_ins(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "and"){
                and_ins(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "sll"){
                int shift = stoi(sa, 0, 2);
                sll_and_sllv(rd, rt, shift);
                *PC = *PC + 4;
            }
            else if(funct == "sllv"){
                std::bitset<32> bst(*rs);
                std::string str = bst.to_string();
                str = str.substr(27, 5);
                int a = stoi(str, 0, 2);
                sll_and_sllv(rd, rt, a);
                *PC = *PC + 4;
            }
            else if(funct == "srl"){
                int shift = stoi(sa, 0, 2);
                srl_and_srlv(rd, rt, shift);
                *PC = *PC + 4;
            }
            else if(funct == "srlv"){
                srl_and_srlv(rd, rt, rs);
                *PC = *PC + 4;
            }
            else if(funct == "sra"){
                int shift = stoi(sa, 0, 2);
                sra_and_srav(rd, rt, shift);
                *PC = *PC + 4;
            }
            else if(funct == "srav"){
                sra_and_srav(rd, rt, rs);
                *PC = *PC + 4;
            }
            else if(funct == "slt"){
                slt(rd, rs, rt);
                *PC = *PC + 4;
            }
            else if(funct == "sltu"){
                sltu(rd, rs, rt);
                *PC = *PC + 4;
            }
            else{                               // system call
                int target_int;
                int index;
                std::string target_str;
                std::string res = "";
                char digit;
                char* ptr;
                // Some file operation variable is set here.
                int fileDescripter;
                char* open_ptr = (*a0 - 0x400000) + (char*)textSegmentStart;        // used for open() syscall
                char* buf = (*a1 - 0x400000) + (char*)textSegmentStart;             // used for read() and write()
                switch (*v0){
                    case 1:                                                         // print_int
                        output << *a0;
                        break;
                    case 4:                                                         // print_string
                        target_int = *a0;
                        ptr = (char*)textSegmentStart + (target_int - 0x400000);
                        while (*ptr != '\0'){
                            output << *(char*)ptr;
                            ptr = ptr + 1;
                        }
                        break;
                    case 5:                                                         // read_int
                        getline(input, target_str);
                        target_str = target_str.erase(0, target_str.find_first_not_of(' '));
                        target_str = target_str.erase(0, target_str.find_first_not_of('\t'));
                        target_str = target_str.erase(0, target_str.find_first_not_of(' '));
                        target_str = target_str.erase(0, target_str.find_first_not_of('\t'));
                        index = 0;
                        digit = target_str[index];
                        if (digit == '-'){              // Negative numbers.
                            res = res + digit;
                            index += 1;
                            digit = target_str[index];
                        }
                        while (digit != '\0'){
                            if (digit - '0' >= 0 && digit - '0' < 10){
                                res = res + digit;
                            }
                            index += 1;
                            digit = target_str[index];
                        }
                        target_int = stoi(res);
                        *v0 = target_int;
                        break;
                    case 8:                                                         // read_string
                        getline(input, target_str);                                 // *a0 = buffer, *a1 = string.length
                        ptr = (*a0 - 0x400000) + (char*)textSegmentStart;
                        index = 0;
                        while(target_str[index] != '\0'){
                            *ptr = target_str[index];
                            ptr = ptr + 1;
                            index += 1;
                        }
                        *ptr = '\0';
                        break;
                    case 9:                                                         // sbrk()
                        target_int = *a0;
                        ptr = (char*)dynamicData_ptr;
                        dynamicData_ptr = (char*)dynamicData_ptr + target_int;
                        *v0 = (int)(size_t)(ptr - (char*)textSegmentStart) + 0x400000;
                        break;
                     case 10:
                        exec = false;
                        break;
                     case 11:                                                       // print_char
                        ptr = (char*)a0;
                        digit = *ptr;
                        output << digit;
                        break;
                     case 12:                                                       // read_char
                        getline(input, target_str);
                        target_str = target_str.erase(0, target_str.find_first_not_of(' '));
                        target_str = target_str.erase(0, target_str.find_first_not_of('\t'));
                        target_str = target_str.erase(0, target_str.find_first_not_of(' '));
                        target_str = target_str.erase(0, target_str.find_first_not_of('\t'));
                        digit = target_str[0];
                        *(char*)v0 = digit;
                        break;
                     case 13:
                        fileDescripter = open(open_ptr, *a1, *a2);
                        *a0 = fileDescripter;
                        break;
                     case 14:
                        *a0 = read(*a0, buf, *a2);
                        break;
                     case 15:
                        *a0 = write(*a0, buf, *a2);
                        break;
                     case 16:
                        index = close(*a0);
                        if (index != 0){                        // close file detection
                            std::cout << "file close fail" << std::endl;
                            exec = false;
                        }
                        break;
                     case 17:
                        exec = false;
                        break;
                }       // switch end.
                *PC = *PC + 4;
            }
        }else if(opcode == "000010" || opcode == "000011"){
            funct = J_type[opcode];
            std::string target = ins.substr(6, 26);
            if (funct == "j"){
                jump(target, PC);
            }else{
                jump_and_link(target, PC, ra);
            }
        }else{
            funct = I_type[opcode];
            int* rs = reg_map[ins.substr(6, 5)];
            int* rt = reg_map[ins.substr(11, 5)];
            std::string imme = ins.substr(16, 16);
            if (funct == "addi"){
                addi(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "addiu"){
                addiu(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "andi"){
                andi(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "beq"){
                beq(rs, rt, imme, PC);
            }
            else if(funct == "bgez"){
                bgez(rs, imme, PC);
            }
            else if(funct == "bgtz"){
                bgtz(rs, imme, PC);
            }
            else if(funct == "blez"){
                blez(rs, imme, PC);
            }
            else if(funct == "bltz"){
                bltz(rs, imme, PC);
            }
            else if(funct == "bne"){
                bne(rs, rt, imme, PC);
            }
            else if(funct == "lb" || funct == "lbu"
                                  || funct == "lh"
                                  || funct == "lhu"
                                  || funct == "lw"){
                int addr = load(rs, imme);
                if(funct == "lb"){
                    char* pt = (char*)textSegmentStart + (addr - 0x400000);
                    long res = (int)*pt;
                    std::bitset<8> bst(res);
                    std::string bst_str = bst.to_string();
                    if(bst_str[0] == '1'){
                        bst_str = "111111111111111111111111" + bst_str;
                        res = stol(bst_str, 0, 2);
                        res = res - 4294967296;
                        *rt = res;
                    }else{
                        bst_str = "000000000000000000000000" + bst_str;
                        res = stoi(bst_str, 0, 2);
                        *rt = res;
                    }
                }else if(funct == "lbu"){
                    char* pt = (char*)textSegmentStart + (addr - 0x400000);
                    long res = (int)*pt;
                    std::bitset<8> bst(res);
                    std::string bst_str = bst.to_string();
                    bst_str = "000000000000000000000000" + bst_str;
                    res = stoi(bst_str, 0, 2);
                    *rt = res;
                }else if(funct == "lh"){
                    char* pt = (char*)textSegmentStart + (addr - 0x400000);
                    short* pt_sh = (short*)pt;
                    short res = (short)*pt_sh;
                    std::bitset<16> bst(res);
                    std::string bst_str = bst.to_string();
                    if(bst_str[0] == '1'){
                        bst_str = "1111111111111111" + bst_str;
                        long r = stol(bst_str, 0, 2);
                        r = r - 4294967296;
                        *rt = r;
                    }else{
                        bst_str = "0000000000000000" + bst_str;
                        int r = stoi(bst_str, 0, 2);
                        *rt = r;
                    }
                }else if(funct == "lhu"){
                    char* pt = (char*)textSegmentStart + (addr - 0x400000);
                    short* pt_sh = (short*)pt;
                    short res = (short)*pt_sh;
                    std::bitset<16> bst(res);
                    std::string bst_str = bst.to_string();
                    bst_str = "0000000000000000" + bst_str;
                    int sh = stoi(bst_str, 0, 2);
                    *rt = sh;
                }else{
                    char* pt = (char*)textSegmentStart + (addr - 0x400000);
                    int* word = (int*)pt;
                    int res = *word;
                    *rt = res;
                }
                *PC = *PC + 4;
            }
            else if(funct == "lui"){
                lui(rt, imme);
                *PC = *PC + 4;
            }
            else if(funct == "ori"){
                ori(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "sb" || funct == "sh"
                                  || funct == "sw"){
                int addr = store(rs, imme);
                char* pt = (char*)textSegmentStart + (addr - 0x400000);
                if(funct == "sb"){
                    char* ch = (char*)rt;
                    *pt = *ch;
                }else if(funct == "sh"){
                    short* sh_s = (short*)rt;
                    short* sh_d = (short*)pt;
                    *sh_d = *sh_s;
                }else{
                    int* wo = rt;
                    int* wo_d = (int*)pt;
                    *wo_d = *wo;
                }
                *PC = *PC + 4;
            }
            else if(funct == "slti"){
                slti(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "sltiu"){
                sltiu(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "xori"){
                xori(rt, rs, imme);
                *PC = *PC + 4;
            }
            else if(funct == "lwl"){
                int load_addr = lwl_and_lwr(rs, imme);
                std::bitset<32> bst_addr(load_addr);
                std::string addr_str = bst_addr.to_string();
                int n = stoi(addr_str.substr(30, 2), 0, 2);
                int addr_align = load_addr - n;
                char* pt = (char*)textSegmentStart + (addr_align - 0x400000) + n;
                char* wr_ptr = (char*)rt;
                for (int i = 1; i <= 4 - n; i++){
                    *wr_ptr = *pt;
                    pt = pt + 1;
                    wr_ptr = wr_ptr + 1;
                }
                *PC = *PC + 4;
            }
            else if(funct == "lwr"){
                int load_addr = lwl_and_lwr(rs, imme);
                std::bitset<32> bst_addr(load_addr);
                std::string addr_str = bst_addr.to_string();
                int n = stoi(addr_str.substr(30, 2), 0, 2);
                int addr_align = load_addr - n;
                char* pt = (char*)textSegmentStart + (addr_align - 0x400000);
                char* wr_ptr = (char*)rt + (4 - n - 1);
                for (int i = 1; i <= n + 1; i++){
                    *wr_ptr = *pt;
                    pt = pt + 1;
                    wr_ptr = wr_ptr + 1;
                }
                *PC = *PC + 4;
            }
            else if(funct == "swl"){
                int store_addr = swl_and_swr(rs, imme);
                std::bitset<32> bst_addr(store_addr);
                std::string addr_str = bst_addr.to_string();
                int n = stoi(addr_str.substr(30, 2), 0, 2);
                int addr_align = store_addr - n;
                char* pt = (char*)textSegmentStart + (addr_align - 0x400000);
                char* wr_ptr = (char*)rt + (4 - n);
                for (int i = 1; i < 4 - n; i++){
                    *pt = *wr_ptr;
                    wr_ptr = wr_ptr + 1;
                    pt = pt + 1;
                }
                *PC = *PC + 4;
            }
            else{                                   // store word right
                int store_addr = swl_and_swr(rs, imme);
                std::bitset<32> bst_addr(store_addr);
                std::string addr_str = bst_addr.to_string();
                int n = stoi(addr_str.substr(30, 2), 0, 2);
                int addr_align = store_addr - n;
                char* pt = (char*)textSegmentStart + (addr_align - 0x400000) + (4 - n - 1);
                char* wr_ptr = (char*)rt;
                for (int i = 1; i <= n + 1; i++){
                    *pt = *wr_ptr;
                    wr_ptr = wr_ptr + 1;
                    pt = pt + 1;
                }
                *PC = *PC + 4;
            }
        }
        ins_count += 1;
        exe = (int*)textSegmentStart + (*PC - 0x400000) / 4;
    }
    input.close();
    output.close();
    free(textSegmentStart);
    free(reg);
}
