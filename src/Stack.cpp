/**
 * @file
 * @brief Struct functions source
*/
#include <stdio.h>
#include <stdlib.h>

#include "Color_output.h"
#include "Stack.h"

enum errorCode stack_verify(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (!stack)
    {       
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_PTR);                              
        return NO_STACK_PTR;
    }

    if (!stack->data)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_DATA_PTR);
        stack_dump(stream, stack);
        return NO_STACK_DATA_PTR;
    }

    if (stack->size >= stack->capacity)
    {      
        PRINT_LINE(stream, file, func, line);
        print_error(stream, SIZE_OUT_OF_CAPACITY);
        stack_dump(stream, stack);       
        return SIZE_OUT_OF_CAPACITY;
    }

    if (stack->size == SIZE_POISON_VAL)
    {         
        PRINT_LINE(stream, file, func, line);
        print_error(stream, SIZE_NOT_VALID);
        stack_dump(stream, stack);
        return SIZE_NOT_VALID;
    }

    if (stack->capacity == CAPACITY_POISON_VAL)
    {     
        PRINT_LINE(stream, file, func, line);
        print_error(stream, CAPACITY_NOT_VALID);
        stack_dump(stream, stack);
        return CAPACITY_NOT_VALID;
    }

    return NO_ERRORS;
}

enum errorCode stack_ctor(struct Stack* stack, size_t capacity, FILE* stream, const char* file, int line, const char* func)
{
    if (!stack) 
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_PTR);
        return NO_STACK_PTR;
    }

    if (capacity == CAPACITY_POISON_VAL)
    { 
        PRINT_LINE(stream, file, func, line);
        print_error(stream, CAPACITY_NOT_VALID);
        return CAPACITY_NOT_VALID;
    }

    stack->data = (elem_t*) calloc(capacity, sizeof(elem_t));
    if (!stack->data)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_MEMORY);
        stack_dump(stream, stack);
        return NO_MEMORY;
    }

    stack->capacity = capacity;
    stack->size     = 0;

    return NO_ERRORS;
}

enum errorCode stack_dtor(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (!stack)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_PTR);
        return NO_STACK_PTR;
    }

    if (!stack->data)
    {  
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_DATA_PTR);
        stack_dump(stream, stack);
        return NO_STACK_DATA_PTR;
    }

    free(stack->data);
    stack->data     = NULL;
    stack->size     = SIZE_POISON_VAL;
    stack->capacity = CAPACITY_POISON_VAL;

    return NO_ERRORS;
}

enum errorCode stack_realloc(struct Stack* stack)// сначала изменить capacity а потом realloc
{
    if (!stack) return NO_STACK_PTR;

    if (stack->size + 1 == stack->capacity)
    {
        stack->data      = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t) * REALLOC_COEF);
        stack->capacity *= REALLOC_COEF;
    }
    if (stack->size <= (size_t) stack->capacity / (2 * REALLOC_COEF))
    {
        stack->data      = (elem_t*) realloc(stack->data, (size_t) stack->capacity * sizeof(elem_t) / REALLOC_COEF);
        stack->capacity /= REALLOC_COEF;
    }

    if (!stack->data) return NO_MEMORY;

    return NO_ERRORS;
}

enum errorCode stack_push(struct Stack* stack, elem_t value, FILE* stream, const char* file, int line, const char* func)
{
    if (!stack) 
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_PTR);
        return NO_STACK_PTR;
    }

    if (stack->size + 1 == stack->capacity)
    {
        enum errorCode err = stack_realloc(stack);
        if (err)
        {
            PRINT_LINE(stream, file, func, line);
            print_error(stream, err);
            stack_dump(stream, stack);
            return err;
        }
    }

    stack->data[stack->size++] = value;

    return NO_ERRORS;
}

elem_t stack_pop(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    if (!stack)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, NO_STACK_PTR);
        return ELEM_T_POISON;
    }

    if (stack->size == 0)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, EMPTY_STACK);
        stack_dump(stream, stack);
        return ELEM_T_POISON;
    }

    if (stack->size <= (size_t) stack->capacity / 4)
    {
        enum errorCode err = stack_realloc(stack);
        if (err) 
        {
            PRINT_LINE(stream, file, func, line);
            print_error(stream, err);
            stack_dump(stream, stack);
            return ELEM_T_POISON;
        }
    }

    stack->size--;

    elem_t ret = stack->data[stack->size];
    stack->data[stack->size] = ELEM_T_POISON;

    return ret;
}