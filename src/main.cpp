#include <stdio.h>

#include "Color_output.h"
#include "Stack.h"

int main()
{
    struct Stack stk = {};

    STACK_CTOR(&stk, 10);

    STACK_DUMP(&stk);

    STACK_DTOR(&stk);

    return 0;
}