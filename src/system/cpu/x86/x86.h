#pragma once

#include "../cpu.h"
#include "gdt.h"
#include <uefi.h>

namespace kernel {

namespace x86 {

enum class DPL {
    kernel_mode = 0,
    __ring_1 = 1,
    __ring_2 = 2,
    user_mode = 3
};

class X86CPU : public CPU {
private:
    GDT* m_pgdt;
public:
    X86CPU() {
        m_pgdt = new GDT();
        printk("Intialized GDT");
    }
    result initialize() override {
        return m_pgdt->enable();
    }
};

}

}