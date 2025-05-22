#include "uefi.h"


namespace boot {

UEFI_interface::UEFI_interface(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {

}

result UEFI_interface::initialize() {
    return result::error;
}

}