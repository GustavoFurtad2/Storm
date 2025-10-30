#ifndef VARIABLE_H
#define VARIABLE_H

#define MAX_VARS 64
#define MAX_FRAMES 64
#define NAME_LEN 32

#include <stdbool.h>

typedef enum {

    VARTYPE_INT,
    VARTYPE_FLOAT,
    VARTYPE_DOUBLE,
    VARTYPE_BOOL,
    VARTYPE_STRING
} VarType;

typedef union {

    bool isNull;
    int i;
    float f;
    double d;
    bool b;
    char* s;
} VarValue;

typedef struct {

    char* name;
    VarType type;
    VarValue value;
} Variable;

#endif