#include <efi.h>
#include <efilib.h>
#include <fermion.h>
#include "system/system.h"
#include "uefi/uefi.h"
#include "system/cpu/x86/gdt.h"

EXTERN_C EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    // If we want to exit UEFI boot mode, delete this object
    boot::UEFI_interface* uefi_interface = new boot::UEFI_interface(ImageHandle, SystemTable);
    uefi_interface->initialize();
    kernel::System* system = new kernel::System();
    system->initalize();
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    
    /* Store the system table for future use in other functions */
    ST = SystemTable;
    EFI_TIME time;
    ST->RuntimeServices->GetTime(&time, nullptr);
    /* Say hi */
    Status = ST->ConOut->OutputString(ST->ConOut, (CHAR16*)u"hello world"); // EFI Applications use Unicode and CRLF, a la Windows
    if (EFI_ERROR(Status))
        return Status;

    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.

       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;

    /* Now wait until a key becomes available.  This is a simple
       polling implementation.  You could try and use the WaitForKey
       event instead if you like */
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;

    return Status;
}