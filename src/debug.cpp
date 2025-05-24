#include <fermion.h>
#include <uefi.h>
#include <stdarg.h>

namespace kernel {

NORETURN void PANIC(const char* str, ...) {
    boot::UEFI_interface::active_interface()->print_console("PANIC");
    //print_err(str);
    while (1) {}
}

void printk_str(const char* str, ...) {
    boot::UEFI_interface::active_interface()->print_console(str);
}

void printk(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    const char *p = format;

    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 's') {
                const char *str = va_arg(args, const char *);
                kernel::printk_str(str);
            } else if (*p == 'd') {
                int num = va_arg(args, int);
                itoa(num, buffer, 10);
                kernel::printk_str(buffer);
            } else if (*p == 'u') {
                uint64_t num = va_arg(args, uint64_t);
                utoa(num, buffer, 10);
                kernel::printk_str(buffer);
            } else if (*p == 'b') {
                uint64_t num = va_arg(args, uint64_t);
                utoa(num, buffer, 2);
                kernel::printk_str(buffer);
            } else if (*p == 'c') {
                char ch = (char)va_arg(args, int);
                buffer[0] = ch;
                buffer[1] = '\0';
                kernel::printk_str(buffer);
            } else if (*p == 'x' || *p == 'X') {
                uint64_t num = va_arg(args, uint64_t);
                utoa(num, buffer, 16);
                kernel::printk_str(buffer);
            } else if (*p == 'p' || *p == 'P') {
                uintptr_t num = va_arg(args, uintptr_t);
                itoa(num, buffer, 16);
                kernel::printk_str(buffer);
            } else {
                // If unknown format, just print it
                buffer[0] = '%';
                buffer[1] = *p;
                buffer[2] = '\0';
                kernel::printk_str(buffer);
            }
        } else {
            buffer[0] = *p;
            buffer[1] = '\0';
            kernel::printk_str(buffer);
        }
        p++;
    }

    va_end(args);
}


}