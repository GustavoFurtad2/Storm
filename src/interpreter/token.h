#ifndef TOKEN_H
#define TOKEN_H

#define OPERATOR_TABLE_SIZE 16
#define KEYWORD_TABLE_SIZE 12

#include <stdlib.h>
#include <stdbool.h>

typedef enum TokenType {

    TOKEN_IF_KEYWORD,
    TOKEN_DO_KEYWORD,
    TOKEN_ELSE_KEYWORD,
    TOKEN_ELSEIF_KEYWORD,
    TOKEN_WHILE_KEYWORD,
    TOKEN_RETURN_KEYWORD,
    TOKEN_FUNCTION_KEYWORD,
    TOKEN_LEFT_PARENTHESIS_KEYWORD,
    TOKEN_RIGHT_PARENTHESIS_KEYWORD,
    TOKEN_LEFT_BRACKET_KEYWORD,
    TOKEN_RIGHT_BRACKET_KEYWORD,
    TOKEN_COMMA_KEYWORD,

    TOKEN_LITERAL_BOOL,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_NUMBER,
    TOKEN_LITERAL_IDENTIFIER,

    TOKEN_EQUALS_OPERATOR,
    TOKEN_NOT_EQUALS_OPERATOR,
    TOKEN_GREATER_THAN_OPERATOR,
    TOKEN_LESS_THAN_OPERATOR,
    TOKEN_GREATER_THAN_OR_EQUALS_OPERATOR,
    TOKEN_LESS_THAN_OR_EQUALS_OPERATOR,
    TOKEN_OR_OPERATOR,
    TOKEN_NOT_OPERATOR,
    TOKEN_AND_OPERATOR,
    TOKEN_ASSIGN_OPERATOR,
    TOKEN_PLUS_OPERATOR,
    TOKEN_MINUS_OPERATOR,
    TOKEN_MULTIPLY_OPERATOR,
    TOKEN_DIVIDE_OPERATOR,
    TOKEN_MOD_OPERATOR,
    TOKEN_POWER_OPERATOR

} TokenType;

typedef struct {
  
    const char* symbol;
    int precedence;

} Instruction;

typedef struct {

    TokenType type;

    const char* value;
    int precedence;

} Token;

extern Token operatorTable[16];

extern Token keywordTable[12];

typedef struct TokenNode {

    unsigned int line;
    Token token;

    struct TokenNode* nextNode;

} TokenNode;

typedef struct TokenStack {

    Token* tokens;
    int top;
    int capacity;
} TokenStack;

typedef struct TokenQueue {

    Token* tokens;
    int size;
    int capacity;
} TokenQueue;

TokenNode createTokenNode(unsigned int line, char* tokenValue, TokenType tokenType);

TokenStack* createTokenStack(int capacity);

TokenQueue* createTokenQueue(int capacity);

bool isTokenStackEmpty(TokenStack* stack);

bool pushToken(TokenStack* stack, Token token);

Token popToken(TokenStack* stack);

Token peekToken(TokenStack* stack);

bool addTokenToQueue(TokenQueue* queue, Token token);

int getTokenPrecedence(Token token);

bool isTokenLeftAssociative(Token token);

bool isMathOperator(TokenType type);

bool isLogicalOperator(TokenType type);

bool isTokenOperator(TokenType type);

Token createTokenFromString(const char* value, TokenType type);

bool existsNextToken(int i, int queueSize);

Token getNextToken(TokenQueue* tokenQueue, int i);

TokenType identifyTokenType(const char* value);

#endif