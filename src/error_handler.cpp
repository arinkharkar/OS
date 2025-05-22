#include <fermion.h>

namespace kernel {

NORETURN void PANIC(const char* str, ...) {
    //print_err(str);
    while (1) {}
}

}