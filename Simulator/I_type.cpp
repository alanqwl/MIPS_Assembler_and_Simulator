// I-type instructions are implemented in this file.
#include "I_type.h"

void addi(int* rt, int* rs, std::string imme){
    int cons;
    long res;
    if (imme[0] == '1'){
        cons = stol(imme, 0, 2) - 65536;
    }else{
        imme = "0000000000000000" + imme;
        cons = stol(imme, 0, 2);
    }
    res = *rs + cons;
    if (res <= 2147483647 && res >= -2147483648){
        *rt = res;
    }
}

void addiu(int* rt, int* rs, std::string imme){
    int cons;
    imme = "0000000000000000" + imme;
    cons = stoi(imme, 0, 2);
    *rt = *rs + cons;
}

void andi(int* rt, int* rs, std::string imme){
    long res;
    imme = "0000000000000000" + imme;
    std::bitset<32> bst_imme(imme);
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rt;
    bst_rt = bst_rs & bst_imme;
    res = bst_rt.to_ulong();
    if (res > 2147483647){
        *rt = res - 4294967296;
    }else{
        *rt = res;
    }
}

void beq(int* rs, int* rt, std::string imme, int* pc){       // If rs = rt, branch.
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs == *rt){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }
}

void bgez(int* rs, std::string imme, int* pc){       // If rs >= 0, branch.
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs >= 0){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }
}

void bgtz(int* rs, std::string imme, int* pc){      // If rs > 0, branch.
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs > 0){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }
}

void blez(int* rs, std::string imme, int* pc){       // If rs <= 0, branch.
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs <= 0){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }
}

void bltz(int* rs, std::string imme, int* pc){       // If rs < 0, branch.
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs < 0){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }

}

void bne(int* rs, int* rt, std::string imme, int* pc){
    imme = imme + "00";
    int offset;
    if (imme[0] == '1'){
        offset = stoi(imme, 0, 2) - 262144;         // 2^18
    }else{
        offset = stoi(imme, 0, 2);
    }
    int target = *pc + 4;
    target = offset + target;
    if (*rs != *rt){
        *pc = target;
    }else{
        *pc = *pc + 4;
    }
}

int load(int* rs, std::string imme){
    imme = "0000000000000000" + imme;
    int offset = stoi(imme, 0, 2);
    int target = *rs + offset;
    return target;
}

void lui(int* rt, std::string imme){
    imme = imme + "0000000000000000";
    std::bitset<32> bst(imme);
    *rt = bst.to_ulong();
}

void ori(int* rt, int* rs, std::string imme){
    imme = "0000000000000000" + imme;
    std::bitset<32> bst_imme(imme);
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> res;
    res = bst_rs | bst_imme;
    *rt = res.to_ulong();
}

int store(int* rs, std::string imme){
    imme = "0000000000000000" + imme;
    int offset = stoi(imme, 0, 2);
    int target = offset + *rs;
    return target;
}

void slti(int* rt, int* rs, std::string imme){
    long cons;
    // Sign extension.
    if (imme[0] == '1'){
        cons = stol(imme) - 65536;
    }else{
        imme = "0000000000000000" + imme;
        cons = stoi(imme, 0, 2);
    }
    if(*rs < cons){
        *rt = 1;
    }else{
        *rt = 0;
    }
}

void sltiu(int* rt, int* rs, std::string imme){
    imme = "0000000000000000" + imme;
    int cons = stoi(imme, 0, 2);
    if (*rs < cons){
        *rt = 1;
    }else{
        *rt = 0;
    }
}


void xori(int* rt, int* rs, std::string imme){
    imme = "0000000000000000" + imme;
    std::bitset<32> bst_imme(imme);
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rd;
    bst_rd = bst_rs ^ bst_imme;
    unsigned long res = bst_rd.to_ulong();
    if (res > 2147483647){
        *rt = res - 4294967296;
    }else{
        *rt = res;
    }
}

int lwl_and_lwr(int* rs, std::string imme){
    long addr;
    if(imme[0] == '1'){
        imme = "1111111111111111" + imme;
        addr = stol(imme, 0, 2) - 4294967296 + *rs;
    }else{
        imme = "0000000000000000" + imme;
        addr = stoi(imme, 0, 2) + *rs;
    }
    return addr;
}

int swl_and_swr(int* rs, std::string imme){
    long addr;
    if(imme[0] == '1'){
        imme = "1111111111111111" + imme;
        addr = stol(imme, 0, 2) - 4294967296 + *rs;
    }else{
        imme = "0000000000000000" + imme;
        addr = stoi(imme, 0, 2) + *rs;
    }
    return addr;
}


