#include <stdio.h>
#include <stdlib.h>

#include "./interpreter/interpreter.h"

int main() {

    system("title Storm 0.1.0");

    printf("Storm 0.1.0 version - https://github.com/GustavoFurtad2/Storm\n");

    runCode();

    system("pause");

    return 0;
}