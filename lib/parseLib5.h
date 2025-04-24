#ifndef PARSE_LIB_
#define PARSE_LIB_

#include <stdio.h>
#include <stdlib.h>

typedef enum StatusCode { SUCCES, ERMALLOC, EROUTOFRANGE, ERTOOLONG, ERINVALIDARG, ERINVALIDUERINPUT, ERVALIDATION } StatusCode;

void handleMallocError(void);

size_t getLine(char** string, FILE* stream);
size_t readLine(char** string);
size_t readLineWithDialog(char** array, const char* message);
size_t readYNWithDialog(const char* str, int* res);
size_t readUDWithDialog(const char* str, int* res);
size_t readLineWithDialog_v(char** array, const char* message, size_t* length, int (*validate)(char* str, size_t len));

long parseLong(char* startPtr,  char** endPtr);
int readLong(long* res);
int readMultLong(long** array, char separator, size_t* count);
int parseMultLong(long** array, char separator, char* inputString, size_t* count);
long readLongWithDialog(const char* message);
long* readMultLongWithDialog(char separator, const char* message, size_t* count);

int readLong_v(long* res, int (*validator)(long number));
int parseMultLong_v(long** array, char separator, char* inputString, size_t* count, int (*validator)(long number, size_t index));
int readMultLong_v(long** array, char separator, size_t* count, int (*validator)(long number, size_t index));
long readLongWithDialog_v(const char* message, int (*validator)(long number));
long* readMultLongWithDialog_v(char separator, const char* message, size_t* count, int (*validator)(long number, size_t index));

double parseDouble(char* startPtr, char** endPtr);
size_t parseMultDouble(double** array, char separator, char* inputString, size_t* count);
size_t readMultDouble(double** array, char separator, size_t* count);
size_t readMultDoubleWithDialog(double** array, char separator, const char* massege, size_t* count);
size_t readDoubleWithDialog(double* ptr, char* messege);
size_t readDoubleWithDialog_v(double* ptr, char* messege, int (*validator)(double number));
size_t readMultDoubleWithDialog_v(double** array, char separator, const char* message, size_t* count,
                                  int (*validator)(double number, size_t index));

size_t readCharWithDialog_v(char* ptr, char* message, int (*validator)(char number));

int isG0(long num);
size_t readMatrix(double*** mat, size_t* rowCount, size_t* colCount);
size_t randomMatrix(double*** mat, long* rCount, long* cCount, double from, double to);

void* realloc_f(void* __ptr, size_t __size);
#endif
