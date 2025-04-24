#ifndef __INClUDE_M_STACK__
#define __INClUDE_M_STACK__

#define STACK_TYPE(T) \
typedef struct T##StackFrame {\
    T data;\
    struct T##StackFrame* prev;\
} T##StackFrame;\
\
int is##T##StackEmpty(T##StackFrame* stack) { return stack->prev == NULL; }\
\
T##StackFrame* push##T##Stack(T##StackFrame* stack, T data) {\
    T##StackFrame* newFrame = malloc(sizeof(T##StackFrame));\
    newFrame->data = data;\
    newFrame->prev = stack;\
    return newFrame;\
}\
T pop##T##Stack(T##StackFrame** stackPtr) {\
    T##StackFrame* newStack = (*stackPtr)->prev;\
    T toRet = (*stackPtr)->data;\
    free(*stackPtr);\
    *stackPtr = newStack;\
    return toRet;\
}\
\
T##StackFrame* create##T##Stack(void) {\
    T##StackFrame* stack = malloc(sizeof(T##StackFrame));\
    stack->prev = NULL;\
    stack->data = 0;\
    return stack;\
}\
\
void free##T##Stack(T##StackFrame* stack) {\
    T##StackFrame* t = NULL;\
    while (stack->prev) {\
        t = stack->prev;\
        free(stack);\
        stack = t;\
    }\
    free(stack);\
}\
\
T peek##T##Stack(T##StackFrame* stack) { return stack->data; }\


typedef struct StackFrame {
    int data;
    struct StackFrame* prev;
} StackFrame;

int isEmpty(StackFrame* stack);
StackFrame* pushStack(StackFrame* stack, int data);
int popStack(StackFrame** stackPtr);
StackFrame* createStack(void);
void freeStack(StackFrame* stack);
void printStack(StackFrame* stack);
int peekStack(StackFrame* stack);
StackFrame* mergeStacks(StackFrame* highToLowStack, StackFrame* lowToHighStack);


#endif