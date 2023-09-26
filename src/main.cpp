#include <stdio.h>

#include "Color_output.h"
#include "Stack.h"

int main()
{
    struct Stack stk = {};

    STACK_CTOR(&stk, 1);

    STACK_PUSH(&stk, 17);

    STACK_DTOR(&stk);

    return 0;
}