#include <stdio.h>

#include "Color_output.h"
#include "Stack.h"

int main()
{
    struct Stack stk = {};

    STACK_CTOR(&stk, 3);

    STACK_VERIFY(&stk);

    STACK_PUSH(&stk, 12);

    printf("%d\n", STACK_POP(&stk));
    STACK_POP(&stk);
    
    STACK_DTOR(&stk);

    return 0;
}