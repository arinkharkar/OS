#pragma once
#include <fermion.h>
#include <fstring>
#include "cpu/cpu.h"
#include "cpu/x86/x86.h"
#include "memory/memory.h"

namespace kernel
{

class System {
private:
    CPU* m_pCPU;
    Memory* m_pmemory;

public:
    System();
    ~System();

    result initalize();

    fstd::string get_cpu_name();
    size_t get_total_physical_memory();
}; 


}



