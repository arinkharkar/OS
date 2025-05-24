#pragma once

#include <stddef.h>
#include <stdint.h>

typedef uint16_t wchar;

static size_t strlen(const wchar* str) {
    size_t i = 0;
    while (*str != 0) ++i;
    return i;
}

static wchar* strcpy(wchar* dest, const wchar* src) {
    size_t i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = 0;
    return dest;
}

static char* wtoa_strcpy(char* dest, const wchar* src) {
    size_t i = 0;
    while (src[i] != 0) {
        dest[i] = (char)src[i];
        ++i;
    }
    dest[i] = 0;
    return dest;
}

static wchar* atow_strcpy(wchar* dest, const char* src) {
    size_t i = 0;
    while (src[i] != 0) {
        dest[i] = (char)src[i];
        ++i;
    }
    dest[i] = 0;
    return dest;
}