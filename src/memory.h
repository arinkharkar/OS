#pragma once

#include <stddef.h>


void* malloc(size_t size);

void free(void* ptr);


void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}


void* memset(void* ptr, int value, size_t num);

void* memcpy(void* dest, const void* src, size_t n);

char* strcpy(char* dest, const char* src);

size_t strlen(const char* s);

