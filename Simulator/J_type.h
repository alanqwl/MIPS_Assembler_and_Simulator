#ifndef J_TYPE_H
#define J_TYPE_H


#include <string>
#include <bitset>

void jump(std::string target, int* pc);

void jump_and_link(std::string target, int* pc, int* ra);

#endif // J_TYPE_H
