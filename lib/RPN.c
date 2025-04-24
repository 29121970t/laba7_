#include <stdlib.h>
#include <stdio.h>

#include "mStack.h"
#include "strLib.h"
#include "parseLib5.h"

STACK_TYPE(double)
STACK_TYPE(char)



int getPriority(char c) {
    switch (c) {
    case '+':
    case '-':
        return 4;
        break;
    case '*':
    case '/':
        return 3;
        break;
    case '^':
        return 2;
        break;
    case '_':
        return 1;
        break;
    case '(':
        return 99;
    case ')':
        return 99;
        break;
    default:
        return 0;
        break;
    }
}

void _printStack(StackFrame* stack) {
    if (!stack->prev) printf("Stack is empty");
    while (stack->prev) {
        printf("%c ", stack->data);
        stack = stack->prev;
    }
    puts("");
}
int isOperation(char c) {
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '!':
    case '(':
    case ')':
    case '_':
        return 1;
        break;

    default:
        return 0;
        break;
    }
}
int isValidUDoubleChar(char c) { return isDigit(c) || c == '.'; }
char* toRevercePolishNotation(char* str, size_t length) {
    charStackFrame* OperationStack = createcharStack();
    char* resultStr = calloc(length * 2, sizeof(char));
    size_t cursor = 0;
    for (size_t i = 0; i < length && str[i]; i++) {
        if (str[i] == '(') {
            OperationStack = pushcharStack(OperationStack, '(');
            continue;
        }
        if (str[i] == ')') {
            while (peekcharStack(OperationStack) != '(') {
                resultStr[cursor++] = popcharStack(&OperationStack);
            }
            popcharStack(&OperationStack);
            continue;
        }
        if (str[i] == '-' && (!i || (str[i-1] != ')' && isOperation(str[i - 1])))) {
            str[i] = '_';
        }


        int priority = getPriority(str[i]);
        if (!priority) {
            resultStr[cursor++] = str[i];
            if (!isValidUDoubleChar(str[i + 1])) resultStr[cursor++] = ' ';
            continue;
        }
        while (!ischarStackEmpty(OperationStack) && getPriority(peekcharStack(OperationStack)) <= priority) {
            resultStr[cursor] = popcharStack(&OperationStack);
            cursor++;
        }
        OperationStack = pushcharStack(OperationStack, str[i]);
    }
    while (!ischarStackEmpty(OperationStack)) {
        resultStr[cursor] = popcharStack(&OperationStack);
        cursor++;
    }
    freecharStack(OperationStack);
    return resultStr;
}

double add(doubleStackFrame** stack) {
    double op1 = popdoubleStack(stack);
    double op2 = popdoubleStack(stack);
    return op1 + op2;
}
double sub(doubleStackFrame** stack) {
    double op1 = popdoubleStack(stack);
    double op2 = popdoubleStack(stack);
    return op2 - op1;
}
double mult(doubleStackFrame** stack) {
    double op1 = popdoubleStack(stack);
    double op2 = popdoubleStack(stack);
    return op2 * op1;
}
double divide(doubleStackFrame** stack) {
    double op1 = popdoubleStack(stack);
    double op2 = popdoubleStack(stack);
    return op2 / op1;
}
double unar_min(doubleStackFrame** stack) {
    double op1 = popdoubleStack(stack);
    return -op1;
}

double clalcRPN(char* rpnString) {
    doubleStackFrame* stack = createdoubleStack();
    size_t i = 0;
    while (rpnString[i]) {
        if (isDigit(rpnString[i])) {
            char* retPos = NULL;
            stack = pushdoubleStack(stack, parseDouble(rpnString + i, &retPos));
            if (retPos != rpnString) i = retPos - rpnString;
        }
        while (isSpace(rpnString[i])) {
            i++;
        }

        if (isOperation(rpnString[i])) {
            switch (rpnString[i]) {
            case '+':
                stack = pushdoubleStack(stack, add(&stack));
                break;
            case '-':
                stack = pushdoubleStack(stack, sub(&stack));
                break;
            case '*':
                stack = pushdoubleStack(stack, mult(&stack));
                break;
            case '/':
                stack = pushdoubleStack(stack, divide(&stack));
                break;
            case '_':
                stack = pushdoubleStack(stack, unar_min(&stack));
                break;

            default:
                break;
            }
            i++;

        }
    }
    double toRet = popdoubleStack(&stack);
    freedoubleStack(stack);
    return toRet;
}
int checkCharValidity(char* str) {
    for (size_t i = 0; str[i]; i++) {
        if (!isValidDoubleChar(str[i]) && !isOperation(str[i])) {
            return i;
        }
    }
    return -1;
}
int _chekOperationsOrderSubrt(char c) {
    return c != '-' && c != '(' && !isDigit(c);
}
int chekOperationsOrder(char* str) {
    if (str[0] != '-' && str[0] != '(' && !isDigit(str[0])) return 0;
    for (size_t i = 1; str[i]; i++) {
        switch (str[i]) {
        case '+':
        case '/':
        case '*':
            if (!str[i + 1] || (str[i + 1] != '-' && str[i + 1] != '(' && !isDigit(str[i + 1]))) return i;
            if (!isDigit(str[i - 1]) && str[i - 1] != ')') return i;
            break;
        case ')':
            if (str[i + 1] && !isOperation(str[i + 1])) return i;
            if (!isDigit(str[i - 1]) && str[i - 1] != ')') return i;
            break;
        case '(':
            if (str[i + 1] != '-' && str[i + 1] != '(' && !isDigit(str[i + 1])) return i;
            if (!isOperation(str[i - 1])) return i;
            break;
        default:
            break;
        }
    }
    return -1;
}
int checkBracketsBalance(char* str) {
    int counter = 0;
    size_t lastBraketPosition = 0;
    for (size_t i = 0; str[i]; i++) {
        if (str[i] == '(') {
            counter++;
            lastBraketPosition = i;
        }
        if (str[i] == ')') counter--;
        if (counter < 0) return i;
    }
    return counter == 0 ? -1 : (int)lastBraketPosition;
}
int checkNumberValidity(char* str) {
    int isInNumberFlag = 0;
    int pastDot = 0;
    if (str[0] == '.') return 0;
    for (size_t i = 1; str[i]; i++) {
        if (str[i] == '.') pastDot++;
        if (isDigit(str[i]) || str[i] == '.') { isInNumberFlag = 1; }
        else { pastDot = 0; isInNumberFlag = 0; }
        if (isInNumberFlag && pastDot > 1) { return i; }
        if (str[i] == '.' && (!str[i + 1] || !isDigit(str[i + 1]) || !isDigit(str[i - 1]))) {
            return i;
        }
    }
    return -1;
}

int validateInputString(char* str1) {
    int errorCode = 0;
    if ((errorCode = checkCharValidity(str1)) > -1 || (errorCode = chekOperationsOrder(str1)) > -1 || (errorCode = checkBracketsBalance(str1)) > -1 || (errorCode = checkNumberValidity(str1)) > -1) {
        return errorCode;
    }
    return -1;

}