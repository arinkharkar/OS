#pragma once

#include <stddef.h>


void* malloc(size_t size);

void free(void* ptr);



class Memory {

};



void* memset(void* ptr, int value, size_t num);

void* memcpy(void* dest, const void* src, size_t n);

char* strcpy(char* dest, const char* src);

size_t strlen(const char* s);

