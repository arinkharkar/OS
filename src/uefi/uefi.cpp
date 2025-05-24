#include <fstring>
#include <stddef.h>
#include "wide_string.h"
#include "uefi.h"


constexpr int g_max_print_length = 2048;

namespace boot {


UEFI_interface* UEFI_interface::m_active_interface = nullptr;


void UEFI_interface::print_console(const char* str) {

    wchar* wide_string = new wchar[g_max_print_length];
    
    atow_strcpy(wide_string, str);
    m_system_table->ConOut->OutputString(m_system_table->ConOut, wide_string);

    delete[] wide_string;
}

void UEFI_interface::print_console(const uint16_t* str) {

    m_system_table->ConOut->OutputString(m_system_table->ConOut, (CHAR16*)str);
}


result UEFI_interface::initialize() {
    print_console("initialized UEFI_interface");
    return result::error;
}

}