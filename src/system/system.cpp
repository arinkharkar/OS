#include "system.h"


namespace kernel {

System::System() {
    #ifdef X86
    m_pCPU = new X86CPU();
    m_pmemory = new Memory();
    #endif
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