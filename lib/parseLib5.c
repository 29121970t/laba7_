#include "parseLib5.h"

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "randomLib.h"
#include "strLib.h"
void handleMallocError(void) {
    puts("MALLOC ERROR. No enougth memory on device.");
    exit(1);
}


int isG0(long num) { return num > 0; }

char* getStatusDescription(size_t code) {
    static char statusDescriptions[8][44] = { "No error occured",
                                             "Could not allocete or enought memory",
                                             "Number is too large or too small",
                                             "The input is too long",
                                             "Function got invalid argument",
                                             "Invalid user input",
                                             "Validation filed. Reread input requirements",
                                             "No such error code" };
    if (code > 7) return statusDescriptions[8];
    return (char*)(&(statusDescriptions[code]));
}
size_t errnoToStatusCode(int errnoCode) {
    switch (errnoCode) {
    case EINVAL:
        return ERINVALIDUERINPUT;
        break;
    case ERANGE:
        return EROUTOFRANGE;
        break;
    default:
        return 0;
        break;
    }
}
int isValidString(const char* str, int (*validator)(char)) {
    for (size_t i = 0; i < ULONG_MAX; i++) {
        if (str[i] == '\0') return 1;
        if (!validator(str[i])) return 0;
    }
    return -1;
}

int isValidString_l(const char* str, size_t length, int (*validator)(char)) {
    for (size_t i = 0; i < length; i++) {
        if (!validator(str[i])) return 0;
    }
    return 1;
}

int isValidString_c(const char* str, int (*validator)(char), char c) {
    for (size_t i = 0; i < ULONG_MAX; i++) {
        if (str[i] == '\0') return 1;
        if (!validator(str[i]) && str[i] != c) return 0;
    }
    return -1;
}

void* realloc_f(void* __ptr, size_t __size) {
    char* tmpPtr = NULL;
    if ((tmpPtr = realloc(__ptr, __size)) == NULL) free(__ptr);
    return tmpPtr;
}
void* realloc_s(void* __ptr, size_t __size) {
    void* result = NULL;
    if ((result = realloc(__ptr, __size)) == NULL) {
        free(__ptr);
        handleMallocError();
    }
    return result;
}

size_t getLine(char** string, FILE* stream) {
    size_t currentSize = 8;  // includes terminator charecter
    size_t counter = 0;
    char* result = NULL;

    if ((result = calloc(currentSize, sizeof(char))) == NULL) handleMallocError();

    while (counter < ULONG_MAX) {
        if (currentSize - counter == 0) {
            currentSize *= 2;
            result = realloc_s(result, sizeof(char) * currentSize);
        }
        result[counter] = getc(stream);
        if (result[counter] == '\n' || result[counter] == EOF) break;
        counter++;
    }
    if (currentSize == ULONG_MAX) {
        free(result);
        return 0;
    }
    result[counter] = '\0';
    currentSize = counter + 1;
    result = realloc_s(result, sizeof(char) * currentSize);

    *string = result;
    return currentSize - 1;
}
size_t ___parseNatNumber(char* startPtr, char** endPtr, size_t threshhold, unsigned digCount) {
    unsigned cursor = 0;
    size_t result = 0;

    for (cursor = 0; cursor < UINT_MAX; cursor++) {
        if (!isDigit(startPtr[cursor])) {
            if (endPtr != NULL) *endPtr = startPtr + cursor;
            return result;
        }

        size_t testMult = result * 10;
        int digit = startPtr[cursor] - '0';

        if (cursor + 1 >= digCount || threshhold - testMult < (size_t)digit) {
            if (endPtr != NULL) *endPtr = startPtr + cursor + 1;
            return threshhold;
        }
        result = testMult + digit;
    }
    if (cursor == UINT_MAX) return threshhold;
    return 0;
}

long parseLong(char* startPtr, char** endPtr) {
    long result = 0;
    int negFlag = 0;
    size_t numberStartIndex = 0;

    while (numberStartIndex < ULONG_MAX) {
        if (!isSpace(startPtr[numberStartIndex])) break;
        numberStartIndex++;
    }

    if (startPtr[numberStartIndex] == '-') {
        negFlag = 1;
        numberStartIndex++;
    }

    result = ___parseNatNumber(startPtr + numberStartIndex, endPtr, LONG_MAX, 20);

    return negFlag ? -result : result;
}
double parseDouble(char* startPtr, char** endPtr) {
    errno = 0;
    double result = 0;
    unsigned cursor = 0;
    int negFlag = 0;
    unsigned digitCounter = 0;
    long dotPos = -1;

    for (; cursor < UINT_MAX; cursor++) {
        if (!isSpace(startPtr[cursor])) break;
    }

    if (startPtr[cursor] == '-') {
        negFlag = 1;
        cursor++;
    }

    for (digitCounter = cursor; digitCounter < UINT_MAX; digitCounter++) {
        if (!isDigit(startPtr[digitCounter])) {
            if (digitCounter == cursor) errno = EINVAL;
            if (dotPos == -1 && startPtr[digitCounter] == '.') {
                dotPos = digitCounter;
                continue;
            }
            if (endPtr != NULL) *endPtr = startPtr + digitCounter;
            break;
        }

        double testMult = result * 10;
        double digit = startPtr[digitCounter] - '0';

        result = testMult + digit;
    }
    if (digitCounter == INT_MAX) {
        errno = ERANGE;
        return 0;
    }

    if (dotPos != -1) {
        for (long i = dotPos + 1; i < digitCounter; i++) {
            result /= 10;
        }
    }

    return negFlag ? -result : result;
}
int readLong(long* res) {
    char* str = NULL;
    if (!getLine(&str, stdin) || !isValidString(str, isValidIntStringChar)) {
        free(str);
        return ERINVALIDUERINPUT;
    }
    *res = parseLong(str, (char**)NULL);
    free(str);
    return SUCCES;
}

int readLong_v(long* res, int (*validator)(long number)) {
    int tempErr = 0;
    if ((tempErr = readLong(res))) return tempErr;
    if (validator && !validator(*res)) return ERVALIDATION;
    return SUCCES;
}


int parseMultLong(long** array, char separator, char* inputString, size_t* count) {
    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter = 0;
    long* result = NULL;

    *count = 0;

    if ((result = calloc(bufferSize, sizeof(long))) == NULL) return ERMALLOC;

    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            bufferSize *= 2;
            result = realloc_s(result, bufferSize * sizeof(long));
        }

        result[counter] = parseLong(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }

        if (tmpPtr2[i] != separator) {
            int toRet = ERINVALIDUERINPUT;
            if (counter == 0) {
                free(result);
                result = NULL;
            }
            if (result && (result = realloc_f(result, counter * sizeof(long))) == NULL) return ERMALLOC;

            if (tmpPtr2[i] == '\0') {
                toRet = SUCCES;
                counter++;
            }

            *count = counter;
            *array = result;

            return toRet;
        }

        tmpPtr1 = tmpPtr2 + 1 + i;
    }
    free(result);
    return ERTOOLONG;
}
int parseMultLong_v(long** array, char separator, char* inputString, size_t* count, int (*validator)(long number, size_t index)) {
    char* tmpPtr1 = inputString;
    char* tmpPtr2 = NULL;

    size_t bufferSize = 8;
    size_t counter = 0;
    long* result = NULL;

    *count = 0;

    if ((result = calloc(bufferSize, sizeof(long))) == NULL) return ERMALLOC;

    for (counter = 0; counter < ULONG_MAX; counter++) {
        if (bufferSize == counter) {
            bufferSize *= 2;
            result = realloc_s(result, bufferSize * sizeof(long));
        }

        result[counter] = parseLong(tmpPtr1, &tmpPtr2);

        unsigned i = 0;
        for (; i < UINT_MAX; i++) {
            if (!isSpace(tmpPtr2[i])) break;
        }

        if (tmpPtr2[i] != separator) {
            int toRet = ERINVALIDUERINPUT;
            if (counter == 0) {
                free(result);
                result = NULL;
            }
            if (result && (result = realloc_f(result, counter * sizeof(long))) == NULL) return ERMALLOC;

            if (tmpPtr2[i] == '\0') {
                toRet = SUCCES;
                counter++;
            }
            if (validator && !validator(result[counter], counter)) toRet = ERVALIDATION;

            *count = counter;
            *array = result;

            return toRet;
        }

        tmpPtr1 = tmpPtr2 + 1 + i;
    }
    free(result);
    return ERTOOLONG;
}

int readMultLong(long** array, char separator, size_t* count) {
    char* str = NULL;
    int tmpError = 0;
    if (!getLine(&str, stdin)) return tmpError;
    if ((tmpError = parseMultLong(array, separator, str, count))) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

int readMultLong_v(long** array, char separator, size_t* count, int (*validator)(long number, size_t index)) {
    char* str = NULL;
    int tmpError = 0;
    if (!getLine(&str, stdin)) return tmpError;
    if ((tmpError = parseMultLong_v(array, separator, str, count, validator))) {
        free(str);
        return tmpError;
    }
    free(str);
    return 0;
}

long* readMultLongWithDialog(char separator, const char* message, size_t* count) {
    int tmpError = 0;
    long* array = NULL;
    do {
        printf("%s", message);
        if ((tmpError = readMultLong(&array, separator, count))) {
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(array);
        }

    } while (tmpError);
    return array;
}

long* readMultLongWithDialog_v(char separator, const char* message, size_t* count, int (*validator)(long number, size_t index)) {
    int tmpError = 0;
    long* array = NULL;
    do {
        printf("%s", message);
        if ((tmpError = readMultLong_v(&array, separator, count, validator))) {
            printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
            free(array);
        }

    } while (tmpError);
    return array;
}

long readLongWithDialog(const char* message) {
    int tmpError = 0;
    long res = 0;
    do {
        printf("%s", message);
        if ((tmpError = readLong(&res))) {
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return res;
}

long readLongWithDialog_v(const char* message, int (*validator)(long number)) {
    int tmpError = 0;
    long res = 0;
    do {
        printf("%s", message);
        if ((tmpError = readLong_v(&res, validator))) {
            printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return res;
}




// // i hate it...

// size_t readChar_v(char* ptr, int (*validator)(char number)) {
//     char* str = NULL;
//     size_t errorTmp = 0;
//     long num = 0;

//     if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
//     if (!isValidString(str, isValidIntString)) {
//         free(str);
//         return ERINVALIDUERINPUT;
//     }

//     num = parseLong(str, NULL);
//     free(str);

//     if (num < -128 || num > 127) return EROUTOFRANGE;
//     if (errno) return errnoToStatusCode(errno);
//     if (validator && !validator((char)num)) return ERVALIDATION;
//     *ptr = num;

//     return 0;
// }
// // this too
// size_t readCharWithDialog_v(char* ptr, char* message, int (*validator)(char number)) {
//     size_t tmpError = 0;
//     do {
//         printf("%s", message);

//         if (tmpError = readChar_v(ptr, validator)) {
//             if (tmpError == ERMALLOC) return ERMALLOC;
//             printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
//         }

//     } while (tmpError);
//     return 0;
// }

// double parseDouble(char* startPtr, char** endPtr) {
//     errno = 0;
//     double result = 0;
//     double shift = .1;
//     unsigned cursor = 0;
//     int negFlag = 0;
//     unsigned digitCounter = 0;
//     long dotPos = -1;

//     for (; cursor < UINT_MAX; cursor++) {
//         if (!isSpace(startPtr[cursor])) break;
//     }

//     if (startPtr[cursor] == '-') {
//         negFlag = 1;
//         cursor++;
//     }

//     for (digitCounter = cursor; digitCounter < UINT_MAX; digitCounter++) {
//         if (!isDigit(startPtr[digitCounter])) {
//             if (digitCounter == cursor) errno = EINVAL;
//             if (dotPos == -1 && startPtr[digitCounter] == '.') {
//                 dotPos = digitCounter;
//                 continue;
//             }
//             if (endPtr != NULL) *endPtr = startPtr + digitCounter;
//             break;
//         }

//         double testMult = result * 10;
//         double digit = startPtr[digitCounter] - '0';

//         result = testMult + digit;
//     }
//     if (digitCounter == INT_MAX) {
//         errno = ERANGE;
//         return 0;
//     }

//     if (dotPos != -1) {
//         for (long i = dotPos + 1; i < digitCounter; i++) {
//             result /= 10;
//         }
//     }

//     return negFlag ? -result : result;
// }
// // refactor step
// size_t readDouble(double* ptr) {
//     char* str = NULL;
//     size_t errorTmp = 0;
//     if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
//     if (!isValidString(str, isValidDoubleStringChar)) {
//         free(str);
//         return ERINVALIDUERINPUT;
//     }

//     *ptr = parseDouble(str, NULL);
//     free(str);

//     if (errno) return errnoToStatusCode(errno);

//     return 0;
// }

// size_t readDouble_v(double* ptr, int (*validator)(double number)) {
//     char* str = NULL;
//     size_t errorTmp = 0;
//     if (errorTmp = getLine(&str, NULL, stdin)) return errorTmp;
//     if (!isValidString(str, isValidDoubleStringChar)) {
//         free(str);
//         return ERINVALIDUERINPUT;
//     }

//     *ptr = parseDouble(str, NULL);
//     free(str);
//     if (errno) return errnoToStatusCode(errno);
//     if (validator && !validator(*ptr)) return ERVALIDATION;

//     return 0;
// }

// size_t _parseMultDoubleExitHanler(size_t counter, double* result, size_t* count, double** array, size_t toRet) {
//     if (counter == 0) {
//         free(result);
//         result = NULL;
//     }

//     if (result && (result = realloc_f(result, counter * sizeof(double))) == NULL) return ERMALLOC;
//     *count = counter;
//     *array = result;
//     return toRet;
// }

// size_t parseMultDouble(double** array, char separator, char* inputString, size_t* count) {
//     if (separator == '.') return ERINVALIDARG;

//     char* tmpPtr1 = inputString;
//     char* tmpPtr2 = NULL;

//     size_t bufferSize = 8;
//     size_t counter;
//     size_t errorTmp = 0;

//     double* result = NULL;

//     if ((result = calloc(bufferSize, sizeof(double))) == NULL) return ERMALLOC;
//     for (counter = 0; counter < ULONG_MAX; counter++) {
//         if (bufferSize == counter) {
//             if ((result = realloc_f(result, bufferSize * 2 * sizeof(double))) == NULL) return ERMALLOC;
//             bufferSize *= 2;
//         }
//         errno = 0;
//         result[counter] = parseDouble(tmpPtr1, &tmpPtr2);

//         unsigned i = 0;
//         for (; i < UINT_MAX; i++) {
//             if (!isSpace(tmpPtr2[i])) break;
//         }

//         if (errno) return _parseMultDoubleExitHanler(counter, result, count, array, errnoToStatusCode(errno));
//         if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
//             return _parseMultDoubleExitHanler(counter, result, count, array, ERINVALIDUERINPUT);

//         if (tmpPtr2[i] == '\0') return _parseMultDoubleExitHanler(++counter, result, count, array, 0);

//         tmpPtr1 = tmpPtr2 + 1 + i;
//     }

//     free(result);
//     return ERTOOLONG;
// }

// size_t parseMultDouble_v(double** array, char separator, char* inputString, size_t* count, int (*validator)(double number, size_t index))
// {
//     if (separator == '.') return ERINVALIDARG;

//     char* tmpPtr1 = inputString;
//     char* tmpPtr2 = NULL;

//     size_t bufferSize = 8;
//     size_t counter;
//     size_t errorTmp = 0;

//     double* result = NULL;

//     if ((result = calloc(bufferSize, sizeof(double))) == NULL) return ERMALLOC;
//     for (counter = 0; counter < ULONG_MAX; counter++) {
//         if (bufferSize == counter) {
//             if ((result = realloc_f(result, bufferSize * 2 * sizeof(double))) == NULL) return ERMALLOC;
//             bufferSize *= 2;
//         }
//         errno = 0;
//         result[counter] = parseDouble(tmpPtr1, &tmpPtr2);

//         unsigned i = 0;
//         for (; i < UINT_MAX; i++) {
//             if (!isSpace(tmpPtr2[i])) break;
//         }

//         if (errno) return _parseMultDoubleExitHanler(counter, result, count, array, errnoToStatusCode(errno));
//         if (tmpPtr2[i] != separator && tmpPtr2[i] != '\0')
//             return _parseMultDoubleExitHanler(counter, result, count, array, ERINVALIDUERINPUT);
//         if (validator && !validator(result[counter], counter))
//             return _parseMultDoubleExitHanler(counter, result, count, array, ERVALIDATION);
//         if (tmpPtr2[i] == '\0') return _parseMultDoubleExitHanler(++counter, result, count, array, 0);

//         tmpPtr1 = tmpPtr2 + 1 + i;
//     }

//     free(result);
//     return ERTOOLONG;
// }

// size_t readMultDouble(double** array, char separator, size_t* count) {
//     char* str = NULL;
//     size_t tmpError = 0;
//     if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
//     if (tmpError = parseMultDouble(array, separator, str, count)) {
//         free(str);
//         return tmpError;
//     }
//     free(str);
//     return 0;
// }

// size_t readMultDouble_v(double** array, char separator, size_t* count, int (*validator)(double number, size_t index)) {
//     char* str = NULL;
//     size_t tmpError = 0;
//     if (tmpError = getLine(&str, NULL, stdin)) return tmpError;
//     if (tmpError = parseMultDouble_v(array, separator, str, count, validator)) {
//         free(str);
//         return tmpError;
//     }
//     free(str);
//     return 0;
// }

// size_t readMultDoubleWithDialog(double** array, char separator, const char* message, size_t* count) {
//     size_t tmpError = 0;
//     do {
//         printf("%s", message);
//         if (tmpError = readMultDouble(array, separator, count)) {
//             if (tmpError == ERMALLOC) return ERMALLOC;
//             printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
//             free(*array);
//         }

//     } while (tmpError);
//     return 0;
// }

// size_t readMultDoubleWithDialog_v(double** array, char separator, const char* message, size_t* count,
//                                   int (*validator)(double number, size_t index)) {
//     size_t tmpError = 0;
//     do {
//         printf("%s", message);
//         if (tmpError = readMultDouble_v(array, separator, count, validator)) {
//             if (tmpError == ERMALLOC) return ERMALLOC;
//             printf("Error occured while parsing %lu-th number: %s\n", *count + 1, getStatusDescription(tmpError));
//             free(*array);
//         }

//     } while (tmpError);
//     return 0;
// }

// size_t readDoubleWithDialog(double* ptr, char* message) {
//     size_t tmpError = 0;
//     do {
//         printf("%s", message);
//         if (tmpError = readDouble(ptr)) {
//             if (tmpError == ERMALLOC) return ERMALLOC;
//             printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
//         }

//     } while (tmpError);
//     return 0;
// }

// size_t readDoubleWithDialog_v(double* ptr, char* message, int (*validator)(double number)) {
//     size_t tmpError = 0;
//     do {
//         printf("%s", message);
//         if (tmpError = readDouble_v(ptr, validator)) {
//             if (tmpError == ERMALLOC) return ERMALLOC;
//             printf("Error occured while parsing number: %s\n", getStatusDescription(tmpError));
//         }

//     } while (tmpError);
//     return 0;
// }

size_t readLine(char** string) {
    return getLine(string, stdin);

}
size_t readLine_v(char** stringPtr, size_t* length, int (*validate)(char* str, size_t len)) {
    size_t len = readLine(stringPtr);
    if (!validate(*stringPtr, len)) return ERVALIDATION;
    if (length) *length = len;
    return 0;
}
size_t readLineWithDialog(char** array, const char* message) {
        printf("%s", message);
        return readLine(array);
}
size_t readLineWithDialog_v(char** array, const char* message, size_t* length, int (*validate)(char* str, size_t len)) {
    size_t tmpError = 0;
    do {
        printf("%s", message);
        if ((tmpError = readLine_v(array, length, validate))) {
            printf("Error occured while parsing input: %s\n", getStatusDescription(tmpError));
        }

    } while (tmpError);
    return 0;
}

// size_t readYNWithDialog(const char* message, int* res) {
//     size_t errorTmp = 0;
//     char* answ1 = NULL;
//     int flag = 0;
//     do {
//         printf("%s", message);
//         if (errorTmp = readLine(&answ1, NULL)) return errorTmp;

//         if (flag = (answ1[0] != 'Y' && answ1[0] != 'N' && answ1[0] != 'y' && answ1[0] != 'n' || answ1[1] != '\0'))
//             puts("ERROR: Cannont interpret this answer. Rered input description");
//         *res = answ1[0] == 'Y' || answ1[0] == 'y';
//         free(answ1);

//     } while (flag);

//     return 0;
// }

// size_t readUDWithDialog(const char* message, int* res) {
//     size_t errorTmp = 0;
//     char* answ1 = NULL;
//     int flag = 0;
//     do {
//         printf("%s", message);
//         if (errorTmp = readLine(&answ1, NULL)) return errorTmp;

//         if (flag = (answ1[0] != 'U' && answ1[0] != 'D' && answ1[0] != 'u' && answ1[0] != 'd' || answ1[1] != '\0'))
//             puts("ERROR: Cannont interpret this answer. Rered input description");
//         *res = answ1[0] == 'u' || answ1[0] == 'U';
//         free(answ1);

//     } while (flag);

//     return 0;
// }

// int isG0(long num) { return num > 0; }

// size_t readMatrix(double*** mat, size_t* rowCount, size_t* colCount) {
//     double** matrix = NULL;
//     size_t rc = 0;
//     size_t cc = 0;
//     char tmpStr[100] = {0};
//     size_t tmpErr = 0;

//     if (tmpErr = readLongWithDialog_v(&rc, "Please enter row count. (r > 0)\n", isG0)) return tmpErr;
//     if ((matrix = calloc(rc, sizeof(double*))) == NULL) return ERMALLOC;
//     if (tmpErr = readMultDoubleWithDialog(matrix, ',', "Please enter first row of a matrix (Real numbers separated by commas)\n", &cc))
//         return tmpErr;

//     for (long i = 1; i < rc; i++) {
//         int flag = 0;
//         size_t tmpColCount = 0;
//         do {
//             sprintf(tmpStr, "Please enter %ld-th row of a matrix: %ld numbers separated by commas.\n", i + 1, cc);
//             if (tmpErr = readMultDoubleWithDialog(&matrix[i], ',', tmpStr, &tmpColCount)) return tmpErr;

//             if (flag = tmpColCount != cc) {
//                 puts("error: Row length mismatch");
//                 free(matrix[i]);
//             }

//         } while (flag);
//     }
//     *rowCount = rc;
//     *colCount = cc;
//     *mat = matrix;
//     return 0;
// }

// size_t randomMatrix(double*** mat, long* rCount, long* cCount, double from, double to) {
//     double** matrix = NULL;
//     size_t tmpErr = 0;
//     long seed = 0;

//     if (tmpErr = readLongWithDialog_v(rCount, "Please enter row count. (r > 0)\n", isG0)) return tmpErr;
//     if (tmpErr = readLongWithDialog_v(cCount, "Please enter clumn count. (r > 0)\n", isG0)) return tmpErr;

//     if ((matrix = calloc(*rCount, sizeof(double*))) == NULL) return ERMALLOC;

//     if (tmpErr = readLongWithDialog_v(&seed, "Please enter seed for random number generation (Natural number)\n", isG0)) return tmpErr;

//     unsigned rand = MrandomUInt(seed);
//     for (long i = 0; i < *rCount; i++) {
//         if ((matrix[i] = calloc(*cCount, sizeof(long))) == NULL) return ERMALLOC;

//         for (int j = 0; j < *cCount; j++) {
//             rand = MrandomUInt(rand);
//             matrix[i][j] = map(0, 254803967, from, to, rand);
//         }
//     }
//     *mat = matrix;
//     return 0;
// }
