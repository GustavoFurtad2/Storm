#include "functionList.h"

void functionPrint(const char* message) {

    printf("%s", message);
}

void functionPrintLine(const char* message) {

    printf("\n%s\n", message);
}

FunctionEntry functionList[] = {

    {"print", functionPrint},
    {"printLine", functionPrintLine},
    {NULL, NULL}
};

bool functionExists(const char* name) {

    for (int i = 0; functionList[i].name != NULL; i++) {

        if (strcmp(functionList[i].name, name) == 0) {

            return true;
        }
    }

    return false;
}

void callFunction(const char* name, const char* arg) {

    for (int i = 0; functionList[i].name != NULL; i++) {

        if (strcmp(functionList[i].name, name) == 0) {

            functionList[i].function(arg);

            return;
        }
    }
}