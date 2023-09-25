#include <stdio.h>

#include "Color_output.h"
#include "Stack.h"

int main()
{
    struct Stack stk = {};

    STACK_CTOR(&stk, 1);

    STACK_DUMP(&stk);

    STACK_PUSH(&stk, 13);

    STACK_DUMP(&stk);
    
    STACK_PUSH(&stk, 13);

    STACK_DUMP(&stk);

    fprintf(stdout, "%d\n", STACK_POP(&stk));

    STACK_DUMP(&stk);

    STACK_DTOR(&stk);

    return 0;
}