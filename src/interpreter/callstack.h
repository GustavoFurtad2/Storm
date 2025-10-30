#ifndef CALLSTACK_H
#define CALLSTACK_H

#include <stdio.h>
#include <string.h>

#include "./variable.h"

#define MAX_VARS 64
#define MAX_FRAMES 64
#define NAME_LEN 32

typedef struct {

    Variable vars[MAX_VARS];
    int varCount;
} Frame;

typedef struct {

    Frame frames[MAX_FRAMES];
    int top;
} CallStack;

void pushFrame(CallStack* stack);
void popFrame(CallStack* stack);
void defineVar(CallStack* stack, const char* name, VarType type, VarValue value);
Variable getVar(CallStack* stack, const char* name);

#endif