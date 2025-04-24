#include <stdio.h>
#include <stdlib.h>


#define GROUTH_FACTOR 2
#define INITIAL_SIZE 8


#define VECTOR_TYPE(T, N)          \
    typedef struct N { \
        size_t length;            \
        size_t allocated;           \
        void (*freeElementFunc)(T);       \
        T *data;             \
    } N;

#define CREATE_VECTOR_TYPE(T, N, L)                         \
    N *create_##N(void (*freeElementFunc)(T) ) {              \
        N *vec = malloc(sizeof(N)); \
        vec->freeElementFunc = freeElementFunc; \
        vec->allocated = L;               \
        vec->data = calloc(L, sizeof(T));             \
        vec->length = 0;                                \
        return vec;                                   \
    }

#define PUSH_VECTOR_TYPE(T, N, G)                                                  \
    void push_##N(N *vec, T data) {                           \
        if (vec->allocated - vec->length == 0) {             \
            vec->data = realloc(vec->data, vec->allocated *= G); \
        }                                                                    \
    vec->data[vec->length++] = data;                                         \
    }

#define POP_VECTOR_TYPE(T, N) \
    T pop_##N(N *vec) {  return vec->data[--(vec->length)]; }

#define INDEX_VECTOR_TYPE(T, N) \
    T index_##N(N *vec, size_t i) { return vec->data[i]; }

#define FREE_VECTOR_TYPE(T, N)                                       \
    void free_##N(N *vec) { \
        if (vec->freeElementFunc) {                                    \
            for (size_t i = 0; i < vec->length; i++) {              \
                vec->freeElementFunc(vec->data[i]);                    \
            }                                                     \
        }                                                         \
        free(vec->data);                                          \
        free(vec);                                                \
    }

#define TO_ARRAY(T, N)                                          \
T * toArray_##N(N* vec){                                            \
    T * arr =  calloc(sizeof(T), vec->length);                  \
    for (size_t i = 0; i < vec->length; i++) {                  \
        arr[i] = index_##N(vec, i);                                  \
    }                                                           \
    return arr;                                                 \
                                                                \
}


#define Vector_persise(ContainedType, TypeName, InitialSize, GrouthRate)   \
    VECTOR_TYPE(ContainedType, TypeName)  \
    CREATE_VECTOR_TYPE(ContainedType, TypeName, InitialSize) \
    PUSH_VECTOR_TYPE(ContainedType, TypeName, GrouthRate)   \
    POP_VECTOR_TYPE(ContainedType, TypeName)    \
    INDEX_VECTOR_TYPE(ContainedType, TypeName)  \
    FREE_VECTOR_TYPE(ContainedType, TypeName)   \
    TO_ARRAY(ContainedType, TypeName)
#define Vector(ContainedType, TypeName)   \
    VECTOR_TYPE(ContainedType, TypeName)  \
    CREATE_VECTOR_TYPE(ContainedType, TypeName, INITIAL_SIZE) \
    PUSH_VECTOR_TYPE(ContainedType, TypeName, GROUTH_FACTOR)   \
    POP_VECTOR_TYPE(ContainedType, TypeName)    \
    INDEX_VECTOR_TYPE(ContainedType, TypeName)  \
    FREE_VECTOR_TYPE(ContainedType, TypeName)   \
    TO_ARRAY(ContainedType, TypeName)





