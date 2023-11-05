// R-type instructions are implemented here.
#include "R_type.h"

void add_and_addu(int* rd, int* rs, int* rt){
    *rd = *rs + *rt;
}

void sub_and_subu(int* rd, int* rs, int* rt){
    *rd = *rs - *rt;
}

void mult_and_multu(int* rs, int* rt, int* hi, int* lo){
    int res = (*rs) * (*rt);
    std::bitset<64> bst(res);
    std::string val = bst.to_string();
    *hi = stol(val.substr(0, 32));
    *lo = stol(val.substr(32, 32));
}

void div_and_divu(int* rs, int* rt, int* hi, int* lo){
    *lo = (*rs) / (*rt);
    *hi = (*rs) % (*rt);
}

void jal(int* rd, int* rs, int* pc){
    *rd = *pc + 4;
    *pc = *rs;
}

void jr(int* rs, int* pc){
    *pc = *rs;
}

void mfhi(int* rd, int* hi){
    *rd = *hi;
}

void mflo(int* rd, int* lo){
    *rd = *lo;
}

void mthi(int* rs, int* hi){
    *hi = *rs;
}

void mtlo(int* rs, int* lo){
    *lo = *rs;
}

void xor_ins(int* rd, int* rs, int* rt){
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rs ^ bst_rt;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void nor_ins(int* rd, int* rs, int* rt){
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rs | bst_rt;
    bst_rd.flip();
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void or_ins(int* rd, int* rs, int* rt){
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rs | bst_rt;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void and_ins(int* rd, int* rs, int* rt){
    std::bitset<32> bst_rs(*rs);
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rs & bst_rt;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void sll_and_sllv(int* rd, int* rt, int sa){            // This version is for sll instruction.
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rt << sa;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void sll_and_sllv(int* rd, int* rt, int* rs){           // Override this function for sllv instruction.
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rt << *rs;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void srl_and_srlv(int* rd, int* rt, int sa){
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rt >> sa;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void srl_and_srlv(int* rd, int* rt, int* rs){           // Override this function for srlv instruction.
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    bst_rd = bst_rt >> *rs;
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void sra_and_srav(int* rd, int* rt, int sa){
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    int bit = bst_rt[31];
    bst_rd = bst_rt >> sa;
    for (int i = 1; i <= sa; i++){
        bst_rd.set(32 - i, bit);
    }
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void sra_and_srav(int* rd, int* rt, int* rs){
    std::bitset<32> bst_rt(*rt);
    std::bitset<32> bst_rd;
    int bit = bst_rt[31];
    bst_rd = bst_rt >> *rs;
    for (int i = 1; i <= *rs; i++){
        bst_rd.set(32 - i, bit);
    }
    long res = bst_rd.to_ulong();
    if (res > 2147483647){
        res = res - 4294967296;
    }
    *rd = res;
}

void slt(int* rd, int* rs, int* rt){
    if (*rs < *rt){
        *rd = 1;
    }else{
        *rd = 0;
    }
}

void sltu(int* rd, int* rs, int* rt){
    long rs_val, rt_val;
    if (*rs < 0){
        rs_val = *rs + 4294967296;
    }else{
        rs_val = *rs;
    }
    if (*rt < 0){
        rt_val = *rt + 4294967296;
    }else{
        rt_val = *rt;
    }
    if (rs_val < rt_val){
        *rd = 1;
    }else{
        *rd = 0;
    }
}
