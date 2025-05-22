#pragma once

#include "../cpu.h"
#include "gdt.h"

namespace kernel {

enum class DPL {
    kernel_mode = 0,
    __ring_1 = 1,
    __ring_2 = 2,
    user_mode = 3
};

class X86CPU : public CPU {
    gdt::GDT* m_pgdt;
    X86CPU() {
        m_pgdt = new gdt::GDT();
    }
    void initialize() override {
        m_pgdt->enable();
    }
};


}