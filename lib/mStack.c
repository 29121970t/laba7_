#include <stdlib.h>
#include <stdio.h>




typedef struct StackFrame {
    int data;
    struct StackFrame* prev;
} StackFrame;

int isEmpty(StackFrame* stack) { return stack->prev == NULL; }

StackFrame* pushStack(StackFrame* stack, int data) {
    StackFrame* newFrame = malloc(sizeof(StackFrame));
    newFrame->data = data;
    newFrame->prev = stack;
    return newFrame;
}
int popStack(StackFrame** stackPtr) {
    StackFrame* newStack = (*stackPtr)->prev;
    int toRet = (*stackPtr)->data;
    free(*stackPtr);
    *stackPtr = newStack;
    return toRet;
}

StackFrame* createStack(void) {
    StackFrame* stack = malloc(sizeof(StackFrame));
    stack->prev = NULL;
    stack->data = 0;
    return stack;
}

void freeStack(StackFrame* stack) {
    StackFrame* t = NULL;
    while (stack->prev) {
        t = stack->prev;
        free(stack);
        stack = t;
    }
    free(stack);
}
void printStack(StackFrame* stack) {
    if (!stack->prev) printf("Stack is empty");
    while (stack->prev) {
        printf("%d ", stack->data);
        stack = stack->prev;
    }
    puts("");
}
int peekStack(StackFrame* stack) { return stack->data; }



StackFrame* mergeStacks(StackFrame* highToLowStack, StackFrame* lowToHighStack) {
    StackFrame* stack3 = createStack();
    int s1_t = peekStack(highToLowStack);
    while (!isEmpty(lowToHighStack) && peekStack(lowToHighStack) > s1_t) {
        stack3 = pushStack(stack3, popStack(&lowToHighStack));
    }

    while (1) {
        if (!isEmpty(highToLowStack) && !isEmpty(stack3) && peekStack(highToLowStack) >= peekStack(stack3)) {
            lowToHighStack = pushStack(lowToHighStack, popStack(&stack3));
        } else {
            lowToHighStack = pushStack(lowToHighStack, popStack(&highToLowStack));
        }
        if(isEmpty(stack3) || isEmpty(highToLowStack)){
            while (!isEmpty(highToLowStack)) {
                lowToHighStack = pushStack(lowToHighStack, popStack(&highToLowStack));
            }
            while (!isEmpty(stack3)) {
                lowToHighStack = pushStack(lowToHighStack, popStack(&stack3));
            }
            freeStack(highToLowStack);
            freeStack(stack3);
            return lowToHighStack;
        }
        
        
    }
    freeStack(stack3);
    freeStack(highToLowStack);
    return lowToHighStack;
}