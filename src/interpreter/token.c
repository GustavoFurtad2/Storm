#include "./token.h"

extern Token operatorTable[] = {

    {TOKEN_PLUS_OPERATOR, "+", 1},
    {TOKEN_MINUS_OPERATOR, "-", 1},
    {TOKEN_MULTIPLY_OPERATOR, "*", 2},
    {TOKEN_DIVIDE_OPERATOR, "/", 2},
    {TOKEN_MOD_OPERATOR, "%", 2},
    {TOKEN_POWER_OPERATOR, "^", 3},
    {TOKEN_LESS_THAN_OPERATOR, "<", 0},
    {TOKEN_GREATER_THAN_OPERATOR, ">", 0},
    {TOKEN_LESS_THAN_OR_EQUALS_OPERATOR, "<=", 0},
    {TOKEN_GREATER_THAN_OR_EQUALS_OPERATOR, ">=", 0},
    {TOKEN_EQUALS_OPERATOR, "==", 0},
    {TOKEN_NOT_EQUALS_OPERATOR, "!=", 0},
    {TOKEN_OR_OPERATOR, "or", -2},
    {TOKEN_AND_OPERATOR, "and", -1},
    {TOKEN_NOT_OPERATOR, "not", 4},
    {TOKEN_ASSIGN_OPERATOR, "=", 1}
};

extern Token keywordTable[] = {

    {TOKEN_IF_KEYWORD, "if", 0},
    {TOKEN_DO_KEYWORD, "do", 0},
    {TOKEN_ELSE_KEYWORD, "else", 0},
    {TOKEN_ELSEIF_KEYWORD, "elseif", 0},
    {TOKEN_WHILE_KEYWORD, "while", 0},
    {TOKEN_RETURN_KEYWORD, "return", 0},
    {TOKEN_FUNCTION_KEYWORD, "func", 0},
    {TOKEN_LEFT_PARENTHESIS_KEYWORD, "(", 0},
    {TOKEN_RIGHT_PARENTHESIS_KEYWORD, ")", 0},
    {TOKEN_LEFT_BRACKET_KEYWORD, "{", 0},
    {TOKEN_RIGHT_BRACKET_KEYWORD, "}", 0},
    {TOKEN_COMMA_KEYWORD, ",", 0},
};

TokenNode createTokenNode(unsigned int line, char* tokenValue, TokenType tokenType) {

    TokenNode tokenNode;

    tokenNode.line = line;
    tokenNode.nextNode = NULL;

    tokenNode.token.value = tokenValue;
    tokenNode.token.type = tokenType;

    return tokenNode;
}

TokenStack* createTokenStack(int capacity) {

    TokenStack* stack = malloc(sizeof(TokenStack));

    if (!stack) {

        return NULL;
    }

    stack->tokens = malloc(sizeof(Token) * capacity);

    if (!stack->tokens) {

        free(stack);

        return NULL;
    }

    stack->top = -1;
    stack->capacity = capacity;

    return stack;
}

TokenQueue* createTokenQueue(int capacity) {

    TokenQueue* queue = malloc(sizeof(TokenQueue));

    if (!queue) {

        return NULL;
    }

    queue->tokens = malloc(sizeof(Token) * capacity);

    if (!queue->tokens) {

        free(queue);

        return NULL;
    }

    queue->size = 0;
    queue->capacity = capacity;

    return queue;
}

bool isTokenStackEmpty(TokenStack* stack) {

    return stack->top == -1;
}

bool pushToken(TokenStack* stack, Token token) {

    if (stack->top >= stack->capacity - 1) {

        return false;
    }

    stack->tokens[++stack->top] = token;

    return true;
}

Token popToken(TokenStack* stack) {

    if (isTokenStackEmpty(stack)) {

        Token emptyToken = {TOKEN_LITERAL_IDENTIFIER, "", 0};

        return emptyToken;
    }

    return stack->tokens[stack->top--];
}

Token peekToken(TokenStack* stack) {

    if (isTokenStackEmpty(stack)) {

        Token emptyToken = {TOKEN_LITERAL_IDENTIFIER, "", 0};

        return emptyToken;
    }

    return stack->tokens[stack->top];
}

bool addTokenToQueue(TokenQueue* queue, Token token) {

    if (queue->size >= queue->capacity) {
        
        return false;
    }

    queue->tokens[queue->size] = token;

    queue->size++;

    return true;
}

int getTokenPrecedence(Token token) {

    for (unsigned int i = 0; i < sizeof(operatorTable) / sizeof(operatorTable[0]); i++) {

        if (operatorTable[i].type == token.type) {

            return operatorTable[i].precedence;
        }
    }

    return 0;
}

bool isTokenLeftAssociative(Token token) {

    return token.type != TOKEN_POWER_OPERATOR;
}

bool isMathOperator(TokenType type) {

    return type == TOKEN_PLUS_OPERATOR ||
           type == TOKEN_MINUS_OPERATOR ||
           type == TOKEN_MULTIPLY_OPERATOR ||
           type == TOKEN_DIVIDE_OPERATOR ||
           type == TOKEN_MOD_OPERATOR ||
           type == TOKEN_POWER_OPERATOR;
}

bool isLogicalOperator(TokenType type) {

    return type == TOKEN_EQUALS_OPERATOR ||
           type == TOKEN_EQUALS_OPERATOR ||
           type == TOKEN_NOT_EQUALS_OPERATOR ||
           type == TOKEN_GREATER_THAN_OPERATOR ||
           type == TOKEN_LESS_THAN_OPERATOR ||
           type == TOKEN_GREATER_THAN_OR_EQUALS_OPERATOR ||
           type == TOKEN_LESS_THAN_OR_EQUALS_OPERATOR ||
           type == TOKEN_OR_OPERATOR ||
           type == TOKEN_AND_OPERATOR ||
           type == TOKEN_NOT_OPERATOR;
}

bool isTokenOperator(TokenType type) {

    return isMathOperator(type) || isLogicalOperator(type);
}

Token createTokenFromString(const char* value, TokenType type) {

    Token token;

    token.type = type;
    token.value = value;
    token.precedence = getTokenPrecedence(token);

    return token;
}

bool existsNextToken(int i, int queueSize) {

    return i < queueSize;
}

Token getNextToken(TokenQueue* tokenQueue, int i) {

    return tokenQueue->tokens[i + 1];
}

TokenType identifyTokenType(const char* value) {

    for (unsigned int i = 0; i < sizeof(keywordTable) / sizeof(keywordTable[0]); i++) {

        if (strcmp(value, keywordTable[i].value) == 0) {

            return keywordTable[i].type;
        }
    }

    for (unsigned int i = 0; i < sizeof(operatorTable) / sizeof(operatorTable[0]); i++) {

        if (strcmp(value, operatorTable[i].value) == 0) {

            return operatorTable[i].type;
        }
    }

    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {

        return TOKEN_LITERAL_BOOL;
    }

    bool isNumber = true;
    bool hasDot = false;

    for (unsigned int i = 0; value[i] != '\0'; i++) {

        if (value[i] == '.' && !hasDot) {

            hasDot = true;
        }
        else if (value[i] < '0' || value[i] > '9') {

            isNumber = false;

            break;
        }
    }

    if (isNumber && strlen(value) > 0 ) {

        return TOKEN_LITERAL_NUMBER;
    }

    return TOKEN_LITERAL_IDENTIFIER;
}