#ifndef DUMP_H
#define DUMP_H

#include <set>
#include <cstdio>
#include <string>

void checkpoint_memory(int ins_count, std::set<int> checkpoints, void* pointer);
#endif // DUMP_H
