#include "callstack.h"

void pushFrame(CallStack* stack) {

    if (stack->top >= MAX_FRAMES - 1) {

        printf("Stack overflow\n");

        return;
    }

    stack->top++;
    stack->frames[stack->top].varCount = 0;

    printf("Callstack start\n");
}

void popFrame(CallStack* stack) {

    if (stack->top < 0) {

        printf("Stack underflow\n");

        return;
    }

    Frame* frame = &stack->frames[stack->top];

    for (int i = 0; i < frame->varCount; i++) {

        if (frame->vars[i].type == VARTYPE_STRING && frame->vars[i].value.s != NULL) {

            free(frame->vars[i].value.s);
            frame->vars[i].value.s = NULL;
        }

        if (frame->vars[i].name != NULL) {

            free(frame->vars[i].name);
            frame->vars[i].name = NULL;
        }
    }

    stack->top--;

    printf("Callstack finish\n");
}

void defineVar(CallStack* stack, const char* name, VarType type, VarValue value) {

    if (stack->top < 0) {

        return;
    }

    Frame* frame = &stack->frames[stack->top];

    if (frame->varCount >= MAX_VARS) {

        return;
    }

    Variable* var = &frame->vars[frame->varCount++];

    var->name = malloc(strlen(name) + 1);

    if (var->name == NULL) {
        frame->varCount--;
        return;
    }

    strcpy(var->name, name);
    var->type = type;
    var->value = value;

    switch (var->type) {

        case VARTYPE_BOOL:

            printf("\nVariable \"%s\" defined with the value of \"%d\"\n\n", name, var->value.b);

            break;

        case VARTYPE_INT:

            printf("\nVariable \"%s\" defined with the value of \"%d\"\n\n", name, var->value.i);
            break;

        case VARTYPE_STRING:

            printf("\nVariable \"%s\" defined with the value of \"%s\"\n\n", name, var->value.s);
            break;

        default:

            break;
    }
}

Variable getVar(CallStack* stack, const char* name) {

    for (int i = stack->top; i >= 0; i--) {

        Frame* frame = &stack->frames[i];

        for (int j = 0; j < frame->varCount; j++) {

            if (strcmp(frame->vars[j].name, name) == 0) {

                return frame->vars[j];
            }
        }
    }

    printf("variable not found\n");

    Variable nullVariable;

    VarValue nullValue;

    nullValue.isNull = true;

    nullVariable.value = nullValue;

    return nullVariable;
}