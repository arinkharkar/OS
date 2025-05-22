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
public:
    UEFI_interface(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table);

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
