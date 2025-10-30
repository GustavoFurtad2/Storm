#include "executor.h"

typedef struct {

    bool onCallFunction;
    bool onAssignment;
    bool onExpression;
    bool onFunction;
    bool shouldAssignVariable;
    bool crashed;
} Executor;

bool executeCode(TokenQueue* tokenQueue) {

    Executor executor;

    executor.onCallFunction = false;
    executor.onAssignment = false;
    executor.onExpression = false;
    executor.onFunction = false; 

    executor.shouldAssignVariable = false;

    printf("\nEXECUTING CODE\n\n");

    size_t i = 0;

    int openParenthesis = 0;

    executor.crashed = false;

    CallStack callStack;

    callStack.top = - 1;

    pushFrame(&callStack);

    printf("\n\nOutput:\n\n");

    char* varName = malloc(64);

    while (i < tokenQueue->size && !executor.crashed) {

        Token currentToken = tokenQueue->tokens[i];

        Token nextToken = getNextToken(tokenQueue, i);

        switch (currentToken.type) {

            case TOKEN_LEFT_PARENTHESIS_KEYWORD:

                break;

            case TOKEN_RIGHT_PARENTHESIS_KEYWORD:


                break;

            case TOKEN_ASSIGN_OPERATOR: {

                if (!executor.onAssignment) {

                    break;
                }

                if (nextToken.type == TOKEN_LITERAL_NUMBER ||
                    nextToken.type == TOKEN_LITERAL_STRING ||
                    nextToken.type == TOKEN_LITERAL_IDENTIFIER) {

                    executor.onAssignment = false;

                    executor.shouldAssignVariable = true;
                }

                break;
            }
            case TOKEN_LITERAL_BOOL:
            case TOKEN_LITERAL_NUMBER:
            case TOKEN_LITERAL_STRING:
            case TOKEN_LITERAL_IDENTIFIER: {

                if (!existsNextToken(i, tokenQueue->size) && !executor.shouldAssignVariable) {

                    break;
                }

                if (nextToken.type == TOKEN_ASSIGN_OPERATOR && !executor.onAssignment) {

                    varName = currentToken.value;

                    executor.onAssignment = true;

                    break;
                }
                else if (nextToken.type == TOKEN_LITERAL_BOOL ||
                         nextToken.type == TOKEN_LITERAL_NUMBER ||
                         nextToken.type == TOKEN_LITERAL_STRING ||
                         nextToken.type == TOKEN_LITERAL_IDENTIFIER ||
                        isTokenOperator(nextToken.type)) {

                    executor.onAssignment = true;
                    executor.onExpression = true;

                    break;
                }
                else {

                    if (executor.onExpression) {

                        executor.onAssignment = false;
                        executor.onExpression = false;
                        executor.shouldAssignVariable = false;
                    }
                }

                VarValue value;

                switch (currentToken.type) {

                    case TOKEN_LITERAL_BOOL: {

                        if (strcmp(currentToken.value, "true") == 0) {

                            value.b = true;
                        }
                        else if (strcmp(currentToken.value, "false") == 0) {

                            value.b = false;
                        }

                        defineVar(&callStack, varName, VARTYPE_BOOL, value);

                        break;
                    }
                    case TOKEN_LITERAL_NUMBER: {

                        value.i = atoi(currentToken.value);

                        defineVar(&callStack, varName, VARTYPE_INT, value);
                        
                        break;
                    }
                    case TOKEN_LITERAL_STRING: {

                        value.s = strdup(currentToken.value);

                        defineVar(&callStack, varName, VARTYPE_STRING, value);
                        
                        break;
                    }
                    case TOKEN_LITERAL_IDENTIFIER: {

                        break;
                    }
                    default:

                        break;
                }

                executor.onAssignment = false;
                executor.shouldAssignVariable = false;

                break;
            }
            default:

                break;
        
        }

        i++;
    }

    popFrame(&callStack);

    free(varName);

    if (executor.crashed) {

        return false;
    }

    printf("\n");

    return true;
}