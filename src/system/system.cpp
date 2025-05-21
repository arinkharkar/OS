#include "system.h"

using namespace kernel;


System::System() {
    #ifdef X86
    m_pCPU = new X86CPU();
    m_pmemory = new Memory();
    #endif
}

System::~System() {
    delete m_pCPU;
    delete m_pmemory;
}