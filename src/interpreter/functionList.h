#ifndef FUNCTION_LIST_H
#define FUNCTION_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef void (*userFunction) (const char*);

void functionPrint(const char* message);

typedef struct {

    const char* name;
    userFunction function;
} FunctionEntry;

extern FunctionEntry functionList[];

bool functionExists(const char* name);

void callFunction(const char* name, const char* arg);

#endif