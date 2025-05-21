#include <stdint.h>
#include <fermion.h>
#include "memory.h"


constexpr uint32_t HEAP_SIZE = 1048576*5;

char heap[HEAP_SIZE] = {0};
char* usedMemory = heap;

void* malloc(size_t c) {
    void* rVal = usedMemory;
    usedMemory += c;
    if (usedMemory >= &heap[HEAP_SIZE]) {
        PANIC("ERROR: OUT OF HEAP SPACE");
    }
    return rVal;
}

void* calloc(size_t number, size_t size) {
    void* rVal = usedMemory;
    memset(usedMemory, 0, number * size);
    usedMemory += number * size;

    if (usedMemory >= &heap[HEAP_SIZE]) {
        PANIC("ERROR: OUT OF HEAP SPACE");
    }
    return rVal;
}

void free(void* ptr) {}


EXTERN_C void __cxa_throw_bad_array_new_length() {
    PANIC("Exception: Invalid Length Specificied in new[]");
}

EXTERN_C void __cxa_pure_virtual()
{
    PANIC("Exception: Invalid Pure Virtual Function");
}


void* memset(void *ptr, int value, size_t num) {
    size_t *p = (size_t*)ptr;
    uint8_t val = (uint8_t)value;

    for (size_t i = 0; i < num / sizeof(size_t); i++) {
        p[i] = val;
    }

    return ptr;
}


void* memcpy(void* dest, const void* src, size_t n) {
    // Cast pointers to char* for byte-wise operations
    char* d = (char*) dest;
    const char* s = (const char*) src;

    // Align address to 4-byte boundary
    while (((uintptr_t)d & (sizeof(uint32_t) - 1)) && n > 0) {
        *d++ = *s++;
        n--;
    }

    // Copy 4 bytes at a time
    uint32_t* dst = (uint32_t*)d;
    const uint32_t* src32 = (const uint32_t*)s;
    while (n >= sizeof(uint32_t)) {
        *dst++ = *src32++;
        n -= sizeof(uint32_t);
    }

    // Copy remaining bytes
    d = (char*)dst;
    s = (const char*)src32;
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return dest;
}


char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while ((*dest++ = *src++)) {
        // copying continues until the null terminator is copied
    }
    return original_dest;
}


size_t strlen(const char* s) {
    size_t sz = 0;
    while (*s) {
        s++;
        sz++;
    }
    return sz;
}