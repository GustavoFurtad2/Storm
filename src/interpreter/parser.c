#include "./parser.h"

Parser* createParser(TokenQueue* tokenQueue) {

    Parser* parser = malloc(sizeof(Parser));

    if (!parser) {

        return NULL;
    }

    parser->queue = tokenQueue;
    parser->onCallFunction = false;
    parser->onAssignment = false;
    parser->onExpression = false;
    parser->onFunction = false;
    parser->canAssign = false;

    return parser;
}

bool runParser(Parser* parser) {

    printf("\nPARSING\n");

    size_t i = 0;

    int openParenthesis = 0;

    if (i >= parser->queue->size) {
        printf("ta vazio");
    }

    bool crashed = false;

    unsigned int skips = 0;

    while (i < parser->queue->size && !crashed) {

        if (skips > 0) {

            skips--;
            i++;
            
            continue;
        }

        Token currentToken = parser->queue->tokens[i];

        printf("\n %s -> %d\n", currentToken.value, currentToken.type);

        switch (currentToken.type) {

            case TOKEN_LEFT_PARENTHESIS_KEYWORD: {

                openParenthesis++;

                break;
            }
            case TOKEN_RIGHT_PARENTHESIS_KEYWORD: {

                if (currentToken.type == TOKEN_RIGHT_PARENTHESIS_KEYWORD) {
                
                }

                break;
            }
            case TOKEN_ASSIGN_OPERATOR: {

                if (parser->onAssignment) {

                    printf("SYNTAX ERROR: %s \"%s\"\n", existsNextToken(i, parser->queue->size) ? "unexpected symbol" : "value missing to assign", currentToken.value);

                    crashed = true;

                    break;
                }

                if (!parser->canAssign) {
 
                    printf("SYNTAX ERROR: unexpected symbol \"%s\", maybe you were trying to assign a value?\n", currentToken.value);
                
                    crashed = true;

                    break;
                }

                parser->onAssignment = true;

                break;
            }
            case TOKEN_LITERAL_IDENTIFIER: {

                if (!existsNextToken(i, parser->queue->size)) {

                    printf("SYNTAX ERROR: unexpected symbol \"%s\"\n", currentToken.value);

                    crashed = true;
                }

                Token nextToken = getNextToken(parser->queue, i);

                if (nextToken.type == TOKEN_LEFT_PARENTHESIS_KEYWORD) {

                    skips++;

                    parser->onCallFunction = true;
                }
                else if (nextToken.type == TOKEN_RIGHT_PARENTHESIS_KEYWORD) {

                    skips++;

                    parser->onCallFunction = false;
                }
                else if (!parser->onAssignment) {

                    parser->canAssign = true;
                }
                else {

                    parser->canAssign = false;
                    parser->onAssignment = false;
                }

                break;
            }
            default: {

                if (!parser->onAssignment) {

                    printf("SYNTAX ERROR: unexpected symbol \"%s\"\n", currentToken.value);

                    crashed = true;
                }

                if (!existsNextToken(i, parser->queue->size)) {

                    parser->canAssign = false;
                    parser->onAssignment = false;

                    break;
                }

                Token nextToken = getNextToken(parser->queue, i);

                if (!isTokenOperator(nextToken.type) && nextToken.type != TOKEN_LITERAL_NUMBER) {
                    parser->canAssign = false;
                    parser->onAssignment = false;
                }

                break;
            }
        }

        i++;
    }

    if (crashed) {

        return false;
    }

    if (openParenthesis > 0) {

        printf("SYNTAX ERROR: missing parentheses closing\n");

        return false;
    }

    if (parser->onAssignment) {

        printf("SYNTAX ERROR: value missing to assign\n");

        return false;
    }

    printf("\n");

    return true;
}