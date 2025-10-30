#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./token.h"
#include "./variable.h"
#include "./callstack.h"
#include "./functionList.h"

bool executeCode(TokenQueue* tokenQueue);

#endif