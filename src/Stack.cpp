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

    #ifdef USE_CANARY_PROTECTION

    if (stack->leftCanary != CANARY_T_DEFAULT)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | LEFT_CANARY_BAD_VALUE);
    }

    if (stack->rightCanary != CANARY_T_DEFAULT)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | RIGHT_CANARY_BAD_VALUE);
    }

    if (*((canary_t*) stack->data) != CANARY_T_DEFAULT)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | LEFT_DATA_CANARY_BAD_VALUE);
    }

    stack->data = (elem_t*) (((canary_t*) stack->data) + 1);
    if (*((canary_t*) (stack->data + stack->capacity)) != CANARY_T_DEFAULT)
    {
        stack->stackErrors = (errorCode) (stack->stackErrors | RIGHT_DATA_CANARY_BAD_VALUE);
    }
    stack->data = (elem_t*) (((canary_t*) stack->data) - 1);

    #endif

    if (stack->stackErrors) stack_dump(stream, stack, file, func, line);


    return stack->stackErrors;
}

enum errorCode stack_ctor(struct Stack* stack, size_t capacity, FILE* stream, const char* file, int line, const char* func)
{
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (capacity <= 0)
    { 
        PRINT_LINE(stream, file, func, line);
        print_error(stream, CAPACITY_NOT_VALID);
        return CAPACITY_NOT_VALID;
    }

    #endif

    #ifdef USE_CANARY_PROTECTION

    while ((capacity * sizeof(elem_t)) % (sizeof(canary_t)) != 0) capacity++;

    stack->data = (elem_t*) calloc(capacity*sizeof(elem_t) + 2*sizeof(canary_t), sizeof(char));

    #else

    stack->data = (elem_t*) calloc(capacity, sizeof(elem_t));

    #endif
    
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack->data, NO_MEMORY, file, func, line)) return NO_MEMORY;

    #endif

    stack->capacity = capacity;
    stack->size     = 0;

    #ifdef USE_CANARY_PROTECTION

    stack->data = (elem_t*) ((canary_t*) stack->data + 1);

    *((canary_t*) stack->data - 1) = CANARY_T_DEFAULT;

    *((canary_t*) (stack->data + stack->capacity)) = CANARY_T_DEFAULT;

    for (size_t i = 0; i < capacity; i++)
    {
        stack->data[i] = ELEM_T_POISON;
    }

    stack->data = (elem_t*) ((canary_t*) stack->data - 1);

    #else

    for (size_t i = 0; i < capacity; i++)
    {
        stack->data[i] = ELEM_T_POISON;
    }

    #endif
    
    #ifdef USE_CANARY_PROTECTION

    stack->leftCanary  = CANARY_T_DEFAULT;
    stack->rightCanary = CANARY_T_DEFAULT;

    #endif

    #ifndef NO_DEBUG

    return stack_verify(stack, stream, file, line, func);

    #else

    return NO_ERRORS;

    #endif
}

enum errorCode stack_dtor(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (stack_verify(stack, stream, file, line, func) == NO_STACK_PTR) return NO_STACK_PTR;

    if ((stack->stackErrors & NO_STACK_DATA_PTR)) 
    {
        print_error(stream, NO_STACK_DATA_PTR);
        abort();
    }

    #endif

    free(stack->data);
    stack->data                    = NULL;
    stack->size                    = SIZE_POISON_VAL;
    stack->capacity                = CAPACITY_POISON_VAL;
    stack->stackHomeland.stackName = NULL;
    stack->stackHomeland.file      = NULL;
    stack->stackHomeland.function  = NULL;
    stack->stackHomeland.line      = -1;

    #ifdef USE_CANARY_PROTECTION

    stack->leftCanary              = CANARY_T_POISON;
    stack->rightCanary             = CANARY_T_POISON;

    #endif

    return NO_ERRORS;
}

enum errorCode stack_realloc(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)//TODO сначала изменить capacity а потом realloc
{
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack, NO_STACK_PTR, __FILE__, __func__, __LINE__)) return NO_STACK_PTR;

    #endif

    if (stack->size + 1 == stack->capacity)
    {
        stack->capacity *= REALLOC_COEF;
    }
    else if (stack->size <= (size_t) stack->capacity / (2 * REALLOC_COEF))
    {
        stack->capacity /= REALLOC_COEF;
    }

    #ifdef USE_CANARY_PROTECTION

    stack->data = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    stack->data = (elem_t*) ((canary_t*) stack->data + 1);
    for (size_t i = stack->size; i < stack->capacity; i++)
    {
        stack->data[i] = ELEM_T_POISON;
    }
    
    *((canary_t*) (stack->data + stack->capacity)) = CANARY_T_DEFAULT;
    stack->data = (elem_t*) ((canary_t*) stack->data - 1);

    #else

    stack->data = (elem_t*) realloc(stack->data, stack->capacity * sizeof(elem_t));

    for (size_t i = stack->size; i < stack->capacity; i++)
    {
        stack->data[i] = ELEM_T_POISON;
    }

    #endif

    #ifndef NO_DEBUG

    return stack_verify(stack, stream, file, line, func);

    #else

    return NO_ERRORS;

    #endif
}

enum errorCode stack_push(struct Stack* stack, elem_t value, FILE* stream, const char* file, int line, const char* func)
{
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    #endif

    if (stack->size + 1 == stack->capacity)
    {
        enum errorCode err = stack_realloc(stack, stream, file, line, func);
        if (err) return err;
    }

    #ifdef USE_CANARY_PROTECTION
    stack->data = (elem_t*) ((canary_t*) stack->data + 1);
    #endif

    stack->data[stack->size++] = value;

    #ifdef USE_CANARY_PROTECTION
    stack->data = (elem_t*) ((canary_t*) stack->data - 1);
    #endif

    #ifndef NO_DEBUG

    return stack_verify(stack, stream, file, line, func);

    #else

    return NO_ERRORS;

    #endif
}

elem_t stack_pop(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
{
    #ifndef NO_DEBUG

    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    #endif

    if (stack->size == 0)
    {
        #ifndef NO_DEBUG

        stack->stackErrors = (errorCode) (stack->stackErrors | EMPTY_STACK);
        stack_dump(stream, stack, file, func, line);

        #endif

        return ELEM_T_POISON;
    }

    if (stack->size <= (size_t) stack->capacity / 4)
    {
        enum errorCode err = stack_realloc(stack, stream, file, line, func);
        if (err) return ELEM_T_POISON;
    }

    stack->size--;

    #ifdef USE_CANARY_PROTECTION
    stack->data = (elem_t*) ((canary_t*) stack->data + 1);
    #endif

    elem_t ret = stack->data[stack->size];
    stack->data[stack->size] = ELEM_T_POISON;

    #ifdef USE_CANARY_PROTECTION
    stack->data = (elem_t*) ((canary_t*) stack->data - 1);
    #endif

    #ifndef NO_DEBUG

    if (stack_verify(stack, stream, file, line, func)) return ELEM_T_POISON;

    #endif

    return ret;
}