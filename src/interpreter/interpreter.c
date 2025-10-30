#include "./interpreter.h"
#include "./token.h"

void runCode() {

    size_t size = 1024;
    size_t len = 0;

    char* buffer = malloc(size);

    if (!buffer) {
        return;
    }

    int character;

    while ((character = getchar()) != '\n' && character != EOF) {

        if (len + 1 >= size) {
            
            size *= 2;
            
            char* temp = realloc(buffer, size);

            if (!temp) {

                free(buffer);
                return;
            }

            buffer = temp;
        }

        buffer[len++] = (char) character;
    }

    buffer[len] = '\0';

    char* temp = realloc(buffer, len + 1);

    if (!temp) {

        free(buffer);

        return;
    }

    char* sourcecode = temp;

    Lexer* lexer = createLexer();
    TokenQueue* tokens = runLexer(lexer, sourcecode);

    if (tokens == NULL || lexer->crashed) {

        freeLexer(lexer, tokens);
        free(sourcecode);
        printf("---FREE: sourcecode\n"); 

        return;
    }

    Parser* parser = createParser(tokens);

    bool sucess = runParser(parser);

    if (!sucess) {

        freeLexer(lexer, tokens);
        free(sourcecode);
        printf("---FREE: sourcecode\n"); 
        
        return;
    }

    sucess = executeCode(tokens);

    if (!sucess) {

        freeLexer(lexer, tokens);
        free(sourcecode);
        printf("---FREE: sourcecode\n"); 
        
        return;
    }

    freeLexer(lexer, tokens);
    free(sourcecode);
    printf("---FREE: sourcecode\n");
}