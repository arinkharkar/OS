# Makefile for building and deploying a UEFI application

# Compiler and flags
CC = x86_64-w64-mingw32-gcc
CXX=x86_64-w64-mingw32-g++
ASM=x86_64-w64-mingw32-as
LD=x86_64-w64-mingw32-gcc
CFLAGS = -ffreestanding -mno-red-zone -fno-pie -fno-exceptions -fno-rtti -I/usr/include/efi -I/usr/include/efi/x86_64 -Isrc/uefi -Isrc/include -Isrc/libccpp -c
ASMFLAGS = -fno-pie
LDFLAGS = -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main

# Paths
BIN=bin
OUT = $(BIN)/fermion.efi
ISO = bootfs.iso
MOUNT_DIR = mnt



# Default target
all: $(BIN)/fermion.efi $(ISO) copy-to-iso

# Find all source files
C_FILES := $(shell find src -type f -name '*.c')
CXX_FILES := $(shell find src -type f -name '*.cpp')
ASM_FILES := $(shell find src -type f -name '*.S')

# Convert source files to object files
OBJ_FILES := $(C_FILES:src/%.c=$(BIN)/%.o) $(ASM_FILES:src/%.S=$(BIN)/%.o) $(CXX_FILES:src/%.cpp=$(BIN)/%.o)

# Ensure bin directories exist
$(shell mkdir -p $(BIN) $(shell find src -type d | sed "s|src|$(BIN)|"))

# Compilation rules
$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(BIN)/%.o: src/%.S
	$(ASM) $< -o $@

# Link the kernel
$(OUT): $(OBJ_FILES)
	$(LD) $(LDFLAGS) $^ -o $@

# Mount ISO, copy BOOTX64.efi
copy-to-iso: $(ISO)
	@mkdir -p $(MOUNT_DIR)
	sudo mount -o loop $(ISO) $(MOUNT_DIR)
	sudo mkdir -p $(MOUNT_DIR)/EFI/BOOT
	sudo cp $(OUT) $(MOUNT_DIR)/EFI/BOOT/BOOTX64.efi
	sudo umount $(MOUNT_DIR)
	@rmdir $(MOUNT_DIR)

# Clean build files
clean:
	rm -rf $(BIN)

.PHONY: all clean copy-to-iso




