#ifndef I_TYPE_H
#define I_TYPE_H

#include <bitset>
#include <string>

void addi(int* rt, int* rs, std::string imme);

void addiu(int* rt, int* rs, std::string imme);

void andi(int* rt, int* rs, std::string imme);

void beq(int* rs, int* rt, std::string imme, int* pc);       // If rs = rt, branch.

void bgez(int* rs, std::string imme, int* pc);       // If rs >= 0, branch.

void bgtz(int* rs, std::string immr, int* pc);

void blez(int* rs, std::string imme, int* pc);       // If rs <= 0, branch.

void bltz(int* rs, std::string imme, int* pc);       // If rs < 0, branch.

void bne(int* rs, int* rt, std::string imme, int* pc);

int load(int* rs, std::string imme);      // Return the target address of the simulated memory.

void lui(int* rt, std::string imme);

void ori(int* rt, int* rs, std::string imme);

int store(int* rs, std::string imme);

void slti(int* rt, int* rs, std::string imme);

void sltiu(int* rt, int* rs, std::string imme);

void xori(int* rt, int* rs, std::string imme);

int lwl_and_lwr(int* rs, std::string imme);

int swl_and_swr(int* rs, std::string imme);
#endif // I_TYPE_H
