#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

typedef struct Lexer {

    bool crashed;

    size_t charIndex;
    size_t sourcecodeLength;
    char* identifierBuffer;

    TokenNode* headToken;
    TokenNode* lastToken;

    bool onString;
    char* stringDelimeter;
    char* stringBuffer;
    bool lastCharacterIsSpace;
    bool makingWord;

    char* sourcecode;
} Lexer;

Lexer* createLexer();
TokenQueue* runLexer(Lexer* lexer, char* sourcecode);
void freeLexer(Lexer* lexer, TokenQueue* tokenQueue);

#endif