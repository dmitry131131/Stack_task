/**
 * @file
 * @brief Struct functions source
*/
#include <stdio.h>
#include <stdlib.h>

#include "Color_output.h"
#include "Stack.h"
#include "OutputSupporting.h"

enum errorCode stack_verify(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (!stack->data)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | NO_STACK_DATA_PTR);
    }

    if (stack->size >= stack->capacity)
    {      
        stack->stackErrors =(errorCode) (stack->stackErrors | SIZE_OUT_OF_CAPACITY);
    }

    if (stack->size == SIZE_POISON_VAL)
    {         
        stack->stackErrors = (errorCode) (stack->stackErrors | SIZE_NOT_VALID);
    }

    if (stack->capacity == CAPACITY_POISON_VAL)
    {     
        stack->stackErrors = (errorCode) (stack->stackErrors | CAPACITY_NOT_VALID);
    }
    
    if (stack->stackErrors) stack_dump(stream, stack, file, func, line);


    return stack->stackErrors;
}

enum errorCode stack_ctor(struct Stack* stack, size_t capacity, FILE* stream, const char* file, int line, const char* func)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (capacity <= 0)
    { 
        PRINT_LINE(stream, file, func, line);
        print_error(stream, CAPACITY_NOT_VALID);
        return CAPACITY_NOT_VALID;
    }

    stack->data = (elem_t*) calloc(capacity, sizeof(elem_t));

    if (no_ptr(stream, stack->data, NO_MEMORY, file, func, line)) return NO_MEMORY;

    for (size_t i = 0; i < capacity; i++)
    {
        stack->data[i] = ELEM_T_POISON;
    }

    stack->capacity = capacity;
    stack->size     = 0;

    return stack_verify(stack, stream, file, line, func);
}

enum errorCode stack_dtor(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (stack_verify(stack, stream, file, line, func) == NO_STACK_PTR) return NO_STACK_PTR;

    if ((stack->stackErrors & NO_STACK_DATA_PTR)) 
    {
        print_error(stream, NO_STACK_DATA_PTR);
        abort();
    }

    free(stack->data);
    stack->data     = NULL;
    stack->size     = SIZE_POISON_VAL;
    stack->capacity = CAPACITY_POISON_VAL;

    return NO_ERRORS;
}

enum errorCode stack_realloc(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)//TODO сначала изменить capacity а потом realloc
{
    if (no_ptr(stream, stack, NO_STACK_PTR, __FILE__, __func__, __LINE__)) return NO_STACK_PTR;

    if (stack->size + 1 == stack->capacity)
    {
        stack->capacity *= REALLOC_COEF;
    }
    if (stack->size <= (size_t) stack->capacity / (2 * REALLOC_COEF))
    {
        stack->capacity /= REALLOC_COEF;
    }

    stack->data = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t));

    return stack_verify(stack, stream, file, line, func);
}

enum errorCode stack_push(struct Stack* stack, elem_t value, FILE* stream, const char* file, int line, const char* func)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (stack->size + 1 == stack->capacity)
    {
        enum errorCode err = stack_realloc(stack, stream, file, line, func);
        if (err) return err;
    }

    stack->data[stack->size++] = value;

    return stack_verify(stack, stream, file, line, func);
}

elem_t stack_pop(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (stack->size == 0)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | EMPTY_STACK);
        stack_dump(stream, stack, file, func, line);
        return ELEM_T_POISON;
    }

    if (stack->size <= (size_t) stack->capacity / 4)
    {
        enum errorCode err = stack_realloc(stack, stream, file, line, func);
        if (err) return ELEM_T_POISON;
    }

    stack->size--;

    elem_t ret = stack->data[stack->size];
    stack->data[stack->size] = ELEM_T_POISON;

    if (stack_verify(stack, stream, file, line, func)) return ELEM_T_POISON;

    return ret;
}