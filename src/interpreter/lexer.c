#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functionList.h"

TokenQueue* shutingYardTokens(TokenNode* tokenList) {

    if (!tokenList) {

        return NULL;
    }

    unsigned int tokenCount = 0;

    TokenNode* currentTokenNode = tokenList;
    
    while (currentTokenNode) {

        tokenCount++;

        currentTokenNode = currentTokenNode->nextNode;
    }

    if (tokenCount == 0) {

        return NULL;
    }

    TokenStack* operatorStack = createTokenStack(tokenCount);
    TokenQueue* outputQueue = createTokenQueue(tokenCount);

    if (!operatorStack || !outputQueue) {

        if (operatorStack) {

            free(operatorStack->tokens);
            free(operatorStack);
        }

        if (outputQueue) {

            free(outputQueue->tokens);
            free(outputQueue);
        }

        return NULL;
    }

    currentTokenNode = tokenList;

    while (currentTokenNode) {

        Token currentToken = currentTokenNode->token;
        TokenType type = currentToken.type;

        if (type == TOKEN_LITERAL_NUMBER || type == TOKEN_LITERAL_IDENTIFIER) {

            addTokenToQueue(outputQueue, currentToken);
        }
        else if (isTokenOperator(type)) {

            while (!isTokenStackEmpty(operatorStack)) {

                Token topToken = peekToken(operatorStack);

                if (isTokenOperator(topToken.type) &&
                    (getTokenPrecedence(topToken) > getTokenPrecedence(currentToken) ||
                    (getTokenPrecedence(topToken) == getTokenPrecedence(currentToken) &&
                    isTokenLeftAssociative(currentToken)))) {

                    Token operator = popToken(operatorStack);

                    addTokenToQueue(outputQueue, operator);
                }
                else {

                    break;
                }
            }

            pushToken(operatorStack, currentToken);
        }
        else if (type == TOKEN_LEFT_PARENTHESIS_KEYWORD) {

            pushToken(operatorStack, currentToken);
        }
        else if (type == TOKEN_RIGHT_PARENTHESIS_KEYWORD) {

            while (!isTokenStackEmpty(operatorStack) &&
                    peekToken(operatorStack).type != TOKEN_LEFT_PARENTHESIS_KEYWORD) {

                Token operator = popToken(operatorStack);

                addTokenToQueue(outputQueue, operator);

                printf("aaa\n");
            }

            if (!isTokenStackEmpty(operatorStack)) {

                popToken(operatorStack);
            }
        }
        else {

            addTokenToQueue(outputQueue, currentToken);
        }

        currentTokenNode = currentTokenNode->nextNode;
    }

    while (!isTokenStackEmpty(operatorStack)) {

        Token operator = popToken(operatorStack);

        addTokenToQueue(outputQueue, operator);
    }

    free(operatorStack->tokens);
    free(operatorStack);

    return outputQueue;
}

void printTokens(TokenQueue* queue) {

    if (!queue) {

        return;
    }

    printf("Expression in postfix notation: ");

    for (int i = 0; i < queue->size; i++) {

        printf("%s ", queue->tokens[i].value);
    }

    printf("\n\n");

    printf("Value   |   TokenType");

    for (int i = 0; i < queue->size; i++) {

        printf("\n %s -> ", queue->tokens[i].value);

        switch (queue->tokens[i].type) {

            case TOKEN_IF_KEYWORD:
            
                printf("IF ");

                break;
         
            case TOKEN_DO_KEYWORD:

                printf("DO ");

                break;
        
            case TOKEN_ELSE_KEYWORD:
                
                printf("ELSE ");

                break;
        
            case TOKEN_ELSEIF_KEYWORD:
            
                printf("ELSEIF ");

                break;
        
            case TOKEN_WHILE_KEYWORD:

                printf("WHILE ");

                break;
        
            case TOKEN_RETURN_KEYWORD:
            
                printf("RETURN ");

                break;
        
            case TOKEN_FUNCTION_KEYWORD:

                printf("FUNC ");

                break;
        
            case TOKEN_LEFT_PARENTHESIS_KEYWORD:

                printf("( ");

                break;
        
            case TOKEN_RIGHT_PARENTHESIS_KEYWORD:

                printf(") ");

                break;
        
            case TOKEN_LEFT_BRACKET_KEYWORD:

                printf("{ ");

                break;
        
            case TOKEN_RIGHT_BRACKET_KEYWORD:

                printf("} ");

                break;
        
            case TOKEN_COMMA_KEYWORD:

                printf(", ");

                break;

            case TOKEN_LITERAL_BOOL:

                printf("bool ");

                break;
        
            case TOKEN_LITERAL_STRING:

                printf("'string' ");

                break;
        
            case TOKEN_LITERAL_NUMBER:

                printf("number ");

                break;
        
            case TOKEN_LITERAL_IDENTIFIER:

                printf("identifier ");

                break;
        
            case TOKEN_EQUALS_OPERATOR:

                printf("== ");

                break;
        
            case TOKEN_NOT_EQUALS_OPERATOR:

                printf("!= ");

                break;
        
            case TOKEN_GREATER_THAN_OPERATOR:

                printf("> ");
                
                break;
        
            case TOKEN_LESS_THAN_OPERATOR:

                printf("< ");

                break;
        
            case TOKEN_GREATER_THAN_OR_EQUALS_OPERATOR:

                printf(">= ");

                break;
        
            case TOKEN_LESS_THAN_OR_EQUALS_OPERATOR:

                printf("<= ");

                break;
        
            case TOKEN_OR_OPERATOR:

                printf("OR ");

                break;
        
            case TOKEN_NOT_OPERATOR:

                printf("NOT ");

                break;
        
            case TOKEN_AND_OPERATOR:

                printf("AND ");

                break;
        
            case TOKEN_ASSIGN_OPERATOR:

                printf("= ");

                break;
        
            case TOKEN_PLUS_OPERATOR:

                printf("+ ");

                break;

            case TOKEN_MINUS_OPERATOR:

                printf("- ");

                break;
        
            case TOKEN_MULTIPLY_OPERATOR:
            
                printf("* ");

                break;
        
            case TOKEN_DIVIDE_OPERATOR:

                printf("/ ");

                break;
        
            case TOKEN_MOD_OPERATOR:

                printf("% ");

                break;
        
            case TOKEN_POWER_OPERATOR:

                printf("^ ");

                break;
        }
    }
}

void freeTokenQueue(TokenQueue* queue) {

    if (!queue) {

        return;
    }

    free(queue->tokens);
    free(queue);
}

void tokenize(Lexer* lexer) {

    char currentChar = lexer->sourcecode[lexer->charIndex];

    if (currentChar == ' ' && !lexer->onString || currentChar == '\t' || currentChar == '\n') {

        if (lexer->makingWord && lexer->identifierBuffer) {

            size_t identifierLength = strlen(lexer->identifierBuffer);

            char* identifierValue = malloc(identifierLength + 1);

            if (identifierValue) {

                strcpy(identifierValue, lexer->identifierBuffer);

                identifierValue[identifierLength] = '\0';

                TokenType tokenType = identifyTokenType(identifierValue);

                TokenNode* newTokenNode = malloc(sizeof(TokenNode));

                if (newTokenNode) {

                    *newTokenNode = createTokenNode(1, identifierValue, tokenType);

                    if (!lexer->headToken) {

                        lexer->headToken = newTokenNode;
                        lexer->lastToken = newTokenNode;
                    }
                    else {

                        lexer->lastToken->nextNode = newTokenNode;
                        lexer->lastToken = newTokenNode;
                    }
                }
            }

            free(lexer->identifierBuffer);

            lexer->identifierBuffer = NULL;
            lexer->makingWord = false;
        }

        lexer->charIndex++;

        return;
    }

    // printf("Current Char %c \n", currentChar);

    switch (currentChar) {

        case '"':
        case '\'':

            if (!lexer->onString) {

                lexer->onString = true;
                lexer->stringDelimeter = currentChar;
                lexer->stringBuffer = NULL;
            }
            else if (lexer->onString && currentChar == lexer->stringDelimeter) {

                lexer->onString = false;

                char* stringValue = lexer->stringBuffer ? lexer->stringBuffer : strdup("");
                TokenNode* stringTokenNode = malloc(sizeof(TokenNode));

                if (!stringTokenNode) {

                    free(stringValue);
                    lexer->crashed = true;

                    return;
                }

                *stringTokenNode = createTokenNode(1, stringValue, TOKEN_LITERAL_STRING);

                if (!lexer->headToken) {
                    lexer->headToken = stringTokenNode;
                    lexer->lastToken = stringTokenNode;
                }
                else {
                    lexer->lastToken->nextNode = stringTokenNode;
                    lexer->lastToken = stringTokenNode;
                }

                lexer->stringBuffer = NULL;
            }

            break;

        case '(':
        case ')':
        case '{':
        case '}':
        case ',':

            if (lexer->makingWord && lexer->identifierBuffer) {

                size_t identifierLength = strlen(lexer->identifierBuffer);

                char* identifierValue = malloc(identifierLength + 1);

                if (identifierValue) {

                    strcpy(identifierValue, lexer->identifierBuffer);

                    identifierValue[identifierLength] = '\0';

                    TokenType tokenType = identifyTokenType(identifierValue);
                    TokenNode* newTokenNode = malloc(sizeof(TokenNode));

                    if (newTokenNode) {

                        *newTokenNode = createTokenNode(1, identifierValue, tokenType);

                        if (!lexer->headToken) {
                            lexer->headToken = newTokenNode;
                            lexer->lastToken = newTokenNode;
                        }
                        else {
                            lexer->lastToken->nextNode = newTokenNode;
                            lexer->lastToken = newTokenNode;
                        }
                    }
                }

                free(lexer->identifierBuffer);
                lexer->identifierBuffer = NULL;
                lexer->makingWord = false;
            }

            char* value = malloc(2);

            if (!value) {

                lexer->crashed = true;
                return;
            }

            value[0] = currentChar;
            value[1] = '\0';

            TokenType type;

            switch (currentChar) {
                
                case '(':

                    type = TOKEN_LEFT_PARENTHESIS_KEYWORD;

                    break;

                case ')':

                    type = TOKEN_RIGHT_PARENTHESIS_KEYWORD;

                    break;

                case '{':

                    type = TOKEN_LEFT_BRACKET_KEYWORD;
                    
                    break;

                case '}':

                    type = TOKEN_RIGHT_BRACKET_KEYWORD;

                default:

                    type = TOKEN_LITERAL_IDENTIFIER;

                    break;
            }

            TokenNode* node = malloc(sizeof(TokenNode));

            if (!node) {

                free(node);

                lexer->crashed = true;

                return;
            }

            printf("VALUE: %s, TOKENIZOU: %d\n", value, type);
            *node = createTokenNode(1, value, type);

            if (!lexer->headToken) {
                lexer->headToken = node;
                lexer->lastToken = node;
                printf("...\n");
            }
            else {
                lexer->lastToken->nextNode = node;
                lexer->lastToken = node;
                printf("mmm\n");

            }

            printf("OPVALUE %s\n", value);

            printf("v: %s, tt: %d\n", value, type);

            break;

        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
        case '=':
        case '<':
        case '>':
        case '!': {

            if (lexer->makingWord && lexer->identifierBuffer) {

                size_t identifierLength = strlen(lexer->identifierBuffer);

                char* identifierValue = malloc(identifierLength + 1);

                if (identifierValue) {

                    strcpy(identifierValue, lexer->identifierBuffer);

                    identifierValue[identifierLength] = '\0';

                    TokenType tokenType = identifyTokenType(identifierValue);

                    TokenNode* newTokenNode = malloc(sizeof(TokenNode));

                    if (newTokenNode) {

                        *newTokenNode = createTokenNode(1, identifierValue, tokenType);

                        if (!lexer->headToken) {

                            lexer->headToken = newTokenNode;
                            lexer->lastToken = newTokenNode;
                        }
                        else {

                            lexer->lastToken->nextNode = newTokenNode;
                            lexer->lastToken = newTokenNode;
                        }
                    }
                }

                free(lexer->identifierBuffer);

                lexer->identifierBuffer = NULL;
                lexer->makingWord = false;
            }

            char operatorString[3] = {currentChar, '\0', '\0'};

            if ((size_t) lexer->charIndex + 1 < lexer->sourcecodeLength) {

                char nextChar = lexer->sourcecode[lexer->charIndex + 1];

                if ((currentChar == '=' && nextChar == '=') ||
                    (currentChar == '!' && nextChar == '=') ||
                    (currentChar == '<' && nextChar == '=') ||
                    (currentChar == '>' && nextChar == '=')) {

                    operatorString[1] = nextChar;

                    lexer->charIndex++;
                }
            }

            char* operatorValue = malloc(strlen(operatorString) + 1);

            if (!operatorValue) {

                lexer->crashed = true;

                return;
            }

            strcpy(operatorValue, operatorString);

            TokenType tokenType = identifyTokenType(operatorValue);

            TokenNode* operatorToken = malloc(sizeof(TokenNode));

            if (!operatorToken) {

                free(operatorValue);

                lexer->crashed = true;

                return;
            }

            *operatorToken = createTokenNode(1, operatorValue, tokenType);

            if(operatorToken == NULL) {

                printf("ÈEEE NULO\n");
            }

            if (!lexer->headToken) {

                lexer->headToken = operatorToken;
                lexer->lastToken = operatorToken;
            }
            else {

                lexer->lastToken->nextNode = operatorToken;
                lexer->lastToken = operatorToken;
            }

            break;
        }

        default: {

            if (lexer->onString) {

                size_t len = lexer->stringBuffer ? strlen(lexer->stringBuffer) : 0;
                
                char* temp = realloc(lexer->stringBuffer, len + 2);

                if (!temp) {

                    lexer->crashed = true;

                    return;
                }

                lexer->stringBuffer = temp;
                lexer->stringBuffer[len] = currentChar;
                lexer->stringBuffer[len + 1] = '\0';
                lexer->charIndex++;

                return;
            }

            // printf("literal %c\n", currentChar);

            size_t newLength = lexer->identifierBuffer ? strlen(lexer->identifierBuffer) : 0;

            char* temp = realloc(lexer->identifierBuffer, newLength + 2);

            if (!temp) {

                lexer->crashed = true;

                return;
            }

            lexer->identifierBuffer = temp;
            lexer->identifierBuffer[newLength] = currentChar;
            lexer->identifierBuffer[newLength + 1] = '\0';
            lexer->makingWord = true;

            break;
        }

    }

    // printf("$%c\n",currentChar);
    // printf("\n--------------\n\n");

    if (!lexer->lastToken) {

        // printf("NULO\n");
    }
    else {
        // printf("L: %c\n", lexer->lastToken->token.value);
    }

    lexer->charIndex++;
}

Lexer* createLexer() {

    Lexer* lexer = malloc(sizeof(Lexer));

    if (!lexer) {
        
        return NULL;
    }

    lexer->crashed = false;
    lexer->charIndex = 0;

    lexer->identifierBuffer = NULL;
    lexer->headToken = NULL;
    lexer->lastToken = NULL;

    lexer->onString = false;
    lexer->stringDelimeter = '\0';
    lexer->stringBuffer = NULL;
    lexer->lastCharacterIsSpace = false;

    lexer->makingWord = false;

    return lexer;
}

TokenQueue* runLexer(Lexer* lexer, char* sourcecode) {

    if (!sourcecode) {

        return NULL;
    }

    lexer->sourcecode = sourcecode;
    lexer->sourcecodeLength = strlen(sourcecode);

    if (lexer->sourcecodeLength == 0) {
        
        return NULL;
    }

    for (size_t i = 0; i < lexer->sourcecodeLength; i++) {

        if (lexer->crashed) {
            break;
        }

        tokenize(lexer);

    }

    if (lexer->onString) {

        printf("SYNTAX ERRROR: unfinished string\n");

        lexer->onString = false;

        lexer->crashed = true;

        if (lexer->stringBuffer) {

            free(lexer->stringBuffer);
            lexer->stringBuffer = NULL;
        }
    }

    if (lexer->makingWord && lexer->identifierBuffer) {

        size_t len = strlen(lexer->identifierBuffer);

        char* value = malloc(len + 1);

        if (value) {

            strcpy(value, lexer->identifierBuffer);

            value[len] = '\0';

            TokenNode* node = malloc(sizeof(TokenNode));

            if (node) {

                *node = createTokenNode(1, value, identifyTokenType(value));

                if (!lexer->headToken) {

                    lexer->headToken = lexer->lastToken = node;
                }
                else {

                    lexer->lastToken->nextNode = node;
                    lexer->lastToken = node;
                }

                printf("FOI\n\n");
                lexer->identifierBuffer[0] = "\0";
            }
        }

        free(lexer->identifierBuffer);

        lexer->identifierBuffer = NULL;
        lexer->makingWord = false;
    }

    TokenQueue* tokens = shutingYardTokens(lexer->headToken);

    printTokens(tokens);

    if (lexer->identifierBuffer) {

        if (lexer->identifierBuffer[0] != '\0') {
            printf("SYNTAX ERRROR: unexpected symbol \"%s\"\n", lexer->identifierBuffer);
            lexer->crashed = true;
        }

        free(lexer->identifierBuffer);
        lexer->identifierBuffer = NULL;
    }

    if (lexer->stringBuffer) {
        free(lexer->stringBuffer);
        lexer->stringBuffer = NULL;
    }

    return tokens;
}

void freeLexer(Lexer* lexer, TokenQueue* tokens) {

    if (!lexer) {

        return;
    }

    TokenNode* currentTokenNode = lexer->headToken;

    while (currentTokenNode) {

        TokenNode* nextNode = currentTokenNode->nextNode;

        if (currentTokenNode->token.value) {
        
            free((char*) currentTokenNode->token.value);
        }

        free(currentTokenNode);

        currentTokenNode = nextNode;
    }

    printf("---FREE: tokenQueue\n");

    if (lexer->identifierBuffer) {
        free(lexer->identifierBuffer);
    }

    if (lexer->stringBuffer) {
        free(lexer->stringBuffer);
    }

    if (tokens) {

        if (tokens->tokens) {

            free(tokens->tokens);
        }

        free(tokens);

        printf("---FREE: tokens\n");
    }

    free(lexer);

    printf("---FREE: lexer\n");
}