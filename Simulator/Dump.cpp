// For memory dumping.
#include "Dump.h"
void checkpoint_memory(int ins_count, std::set<int> checkpoints, void* pointer){
    if(!checkpoints.count(ins_count)){
        return;
    }
    std::string name = "memory_" + std::to_string(ins_count) + ".bin";
    FILE* fd = fopen(name.c_str(), "wb");
    fwrite(pointer, 1, 0x600000, fd);
    fclose(fd);
}
