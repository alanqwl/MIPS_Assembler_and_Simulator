// J-type instructions are implemented here.
#include "J_type.h"

void jump(std::string target, int* pc){
    std::string addr = target + "00";
    int a = *pc;
    std::bitset<32> current_addr(a + 4);
    std::string b = current_addr.to_string();
    addr = b.substr(0, 4) + addr;
    long res = stol(addr, 0, 2);
    *pc = res;
}

void jump_and_link(std::string target, int* pc, int* ra){
    std::string addr = target + "00";
    int a = *pc;
    std::bitset<32> current_addr(a + 4);
    std::string b = current_addr.to_string();
    addr = b.substr(0, 4) + addr;
    long res = stol(addr, 0, 2);
    *ra = a + 4;
    *pc = res;
}
