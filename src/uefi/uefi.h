#pragma once
#include <efi.h>
#include <efilib.h>
#include <fermion.h>


#define TEXT(s) (CHAR16*)(s)

namespace boot
{

class UEFI_interface {
    EFI_HANDLE m_image_handle;
    EFI_SYSTEM_TABLE* m_system_table;

    static UEFI_interface* m_active_interface;

public:
    UEFI_interface(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
        m_image_handle = image_handle;
        m_system_table = system_table;
        m_active_interface = this;
    }
    ~UEFI_interface() {
        m_active_interface = nullptr;
    }

    static UEFI_interface* active_interface() {
        return m_active_interface;
    }

    void print_console(const CHAR16* str);

    void print_console(const char* str);

    result initialize(); 

};


static UINT16* to_uefi_str(const char* input_string, UINT16* uefi_string) {
    int i = 0;
    while (input_string[i] != 0) {
        uefi_string[i] = input_string[i];
        i++;
    }
    return uefi_string;
}



}
