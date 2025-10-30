#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./token.h"

typedef struct Parser {

    TokenQueue* queue;

    bool onCallFunction; 
    bool onAssignment;
    bool onExpression;
    bool onFunction;
    bool canAssign
} Parser;

Parser* createParser(TokenQueue* tokenQueue);
bool runParser(Parser* parser);

#endif