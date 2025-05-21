#pragma once

#include <fstring>
#define EXTERN_C extern "C"

// This just defines NORETURN so that it is standard between C and C++ and across compilers
#define NORETURN [[noreturn]]


// Check we are compiling on the correct system
#ifdef __i386__
#error "Fermion only support x86_64, this compiler is currently using i386 (x86_32)"
#endif

#ifdef __arm__
#error "Fermion only support x86_64, this compiler is currently using ARM (arm32)"
#endif

#ifdef __aarch64__
#error "Fermion currently only supports x86_64, this compiler is currently using AARCH64 (arm64)"
#endif

#ifdef __x86_64__
#define X86
#define X86_64
#endif

#ifdef __aarch64
#define ARM64
#endif

#if (defined(linux) || defined(__APPLE__) || defined(unix) || defined(__FreeBSD__)) && !defined(VSCODE)
#error "Fermion needs a x86_64-w64-mingw32-gcc compiler, this is being compiled for a different system!"
#endif


typedef unsigned char byte;

typedef enum {
    result_error = 0,
    result_success = 1
} result;


NORETURN void PANIC(const char* str, ...);

NORETURN void PANIC(const fstd::string& str, ...);

static inline void disable_interupts() { 
    #ifdef __x86_64__
    asm volatile ("cli"); 
    #endif
}
static inline void enable_interupts() { 
    #ifdef __x86_64__
    asm volatile ("sti"); 
    #endif
}