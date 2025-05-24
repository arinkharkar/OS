#include "system.h"


namespace kernel {

System::System() {
    #ifdef X86
    m_pCPU = new x86::X86CPU();
    #endif

    m_pmemory = new Memory();
}

result System::initalize() {
    m_pCPU->initialize();
    return result::success;
}

System::~System() {
    delete m_pCPU;
    delete m_pmemory;
}

}