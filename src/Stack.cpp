/**
 * @file
 * @brief Struct functions source
*/
#include "Stack.h"

enum errorCode stack_verify(struct Stack* stack)
{
    if (!stack)                         return NO_STACK_PTR;
    if (!stack->data)                   return NO_STACK_DATA_PTR;
    if (stack->size >= stack->capacity) return SIZE_OUT_OF_CAPACITY;
    if (stack->size     == -1)          return SIZE_NOT_VALID;
    if (stack->capacity == -1)          return CAPACITY_NOT_VALID;

    return NO_ERRORS;
}