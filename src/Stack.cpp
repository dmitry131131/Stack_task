/**
 * @file
 * @brief Struct functions source
*/
#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"

enum errorCode stack_verify(struct Stack* stack)
{
    if (!stack)                                     return NO_STACK_PTR;
    if (!stack->data)                               return NO_STACK_DATA_PTR;
    if (stack->size >= stack->capacity)             return SIZE_OUT_OF_CAPACITY;
    if (stack->size     == SIZE_POISON_VAL)         return SIZE_NOT_VALID;
    if (stack->capacity == CAPACITY_POISON_VAL)     return CAPACITY_NOT_VALID;

    return NO_ERRORS;
}

enum errorCode stack_ctor(struct Stack* stack, size_t capacity)
{
    if (!stack)                             return NO_STACK_PTR;
    if (capacity == CAPACITY_POISON_VAL)    return CAPACITY_NOT_VALID;

    stack->data = (elem_t*) calloc(capacity, sizeof(elem_t));
    if (!stack->data) return NO_MEMORY;

    stack->capacity = capacity;
    stack->size     = 0;

    return NO_ERRORS;
}

enum errorCode stack_dtor(struct Stack* stack)
{
    if (!stack)         return NO_STACK_PTR;
    if (!stack->data)   return NO_STACK_DATA_PTR;

    free(stack->data);
    stack->data     = NULL;
    stack->size     = SIZE_POISON_VAL;
    stack->capacity = CAPACITY_POISON_VAL;

    return NO_ERRORS;
}

enum errorCode stack_realloc(struct Stack* stack)
{
    if (!stack) return NO_STACK_PTR;

    if (stack->size + 1 == stack->capacity)
    {
        stack->data      = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t) * REALLOC_COEF);
        stack->capacity *= REALLOC_COEF;
    }
    if (stack->size <= (size_t) stack->capacity / 4)
    {
        stack->data      = (elem_t*) realloc(stack->data, (size_t) stack->capacity * sizeof(elem_t) / REALLOC_COEF);
        stack->capacity /= REALLOC_COEF;
    }

    if (!stack->data) return NO_MEMORY;

    return NO_ERRORS;
}

enum errorCode stack_push(struct Stack* stack, elem_t value)
{
    if (!stack) return NO_STACK_PTR;

    CHECK_ERR(stack_realloc(stack));

    stack->data[stack->size++] = value;

    return NO_ERRORS;
}

elem_t stack_pull(struct Stack* stack)
{
    if (!stack) return NO_STACK_PTR;

    CHECK_ERR(stack_realloc(stack));

    if (stack->size == 0) return EMPTY_STACK;
    stack->size--;

    elem_t ret = stack->data[stack->size];
    stack->data[stack->size] = ELEM_T_POISON;

    return ret;
}