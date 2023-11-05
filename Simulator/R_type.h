#ifndef R_TYPE_H
#define R_TYPE_H

#include <bitset>
#include <string>

void add_and_addu(int* rd, int* rs, int* rt);

void sub_and_subu(int* rd, int* rs, int* rt);

void mult_and_multu(int* rs, int*rt, int* hi, int*lo);

void div_and_divu(int* rs, int* rt, int* hi, int* lo);

void jal(int* rd, int* rs, int* pc);

void jr(int* rs, int* pc);

void mfhi(int* rd, int* hi);

void mflo(int* rd, int* lo);

void mthi(int* rs, int* hi);

void mtlo(int* rs, int* lo);

void xor_ins(int* rd, int* rs, int* rt);

void nor_ins(int* rd, int* rs, int* rt);

void or_ins(int* rd, int* rs, int* rt);

void and_ins(int* rd, int* rs, int* rt);

void sll_and_sllv(int* rd, int* rt, int sa);

void sll_and_sllv(int* rd, int* rt, int* rs);

void srl_and_srlv(int* rd, int* rt, int sa);

void srl_and_srlv(int* rd, int* rt, int* rs);

void sra_and_srav(int* rd, int* rt, int sa);

void sra_and_srav(int* rd, int* rt, int* rs);

void slt(int* rd, int* rs, int* rt);

void sltu(int* rd, int* rs, int* rt);
#endif // R_TYPE_H
