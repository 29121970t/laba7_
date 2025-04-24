#include "strLib.h"

int strcopm(char* s1, char* s2) {
    for (size_t i = 0; s1[i] && s2[i]; i++) {
        if (s1[i] != s2[i]) return 0;
    }
    return 1;
}
void _strcpy(char* dest, char* src) {
    for (size_t i = 0; src[i]; i++) {
        dest[i] = src[i];
    }
}

void* memcpy(void* dest, const void* src, size_t len) {
    char* d = dest;
    const char* s = src;
    while (len--) *d++ = *s++;
    return dest;
}
int isDigit(char c) { return c >= '0' && c <= '9' ? 1 : 0; }
int isValidIntChar(char c) { return isDigit(c) || c == '-'; }
int isSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
int isValidDoubleChar(char c) { return isValidIntChar(c) || c == '.'; }
int isValidDoubleStringChar(char c) { return isValidDoubleChar(c) || isSpace(c); }
int isValidIntStringChar(char c) { return isValidIntChar(c) || isSpace(c); }
