/**
 * @file
 * @brief File with testing functions for stack
*/

#include <stdio.h>

#include "Color_output.h"
#include "Stack.h"

enum errorCode ctor_test(Stack* stack, FILE* stream);
enum errorCode push_test(Stack* stack, FILE* stream);
enum errorCode pop_test(Stack* stack, FILE* stream);
enum errorCode dtor_test(Stack* stack, FILE* stream);


int main()
{
    test_driver(stdout);

    return 0;
}

enum errorCode test_driver(FILE* stream)
{
    Stack stk = {};

    if (ctor_test(&stk, stream)) return stk.stackErrors;

    if (push_test(&stk, stream)) return stk.stackErrors;

    if (pop_test(&stk, stream)) return stk.stackErrors;

    if (dtor_test(&stk, stream)) return stk.stackErrors;

    color_fprintf(stream, COLOR_GREEN, STYLE_BOLD, "Test successfull!\n");

    return NO_ERRORS;
}

enum errorCode ctor_test(Stack* stack, FILE* stream)
{
    STACK_CTOR(stack, 10);

    if (stack->stackErrors)
    {
        color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");
        fprintf(stream, "Ctor test failed!\n");

        return stack->stackErrors;
    }
    return NO_ERRORS;
}

enum errorCode push_test(Stack* stack, FILE* stream)
{
    for (size_t i = 0; i < 1000; i++)
    {
        if (STACK_PUSH(stack, 1000))
        {
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");
            fprintf(stream, "Push test failed!\n");

            return stack->stackErrors;
        }
    }

    return NO_ERRORS;
}

enum errorCode pop_test(Stack* stack, FILE* stream)
{
    for (size_t i = 0; i < 1000; i++)
    {
        if (STACK_POP(stack) != 1000)
        {
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");
            fprintf(stream, "Pop test failed!\n");

            return stack->stackErrors;
        }
    }

    return NO_ERRORS;
}

enum errorCode dtor_test(Stack* stack, FILE* stream)
{
    errorCode err = STACK_DTOR(stack);

    if (err)
    {
        color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");
        fprintf(stream, "Dtor test failed!\n");
        stack->stackErrors = err;

        return err;
    }

    return NO_ERRORS;
}
