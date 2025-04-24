#ifndef ___STR_LIB_DEF
#define ___STR_LIB_DEF

#include <stdlib.h>

int strcopm(char* s1, char* s2);
void _strcpy(char* dest, char* src);
void* memcpy(void* dest, const void* src, size_t len);
int isDigit(char c);
int isValidIntChar(char c);
int isSpace(char c);
int isValidDoubleChar(char c);
int isValidDoubleStringChar(char c);
int isValidIntStringChar(char c);


#endif