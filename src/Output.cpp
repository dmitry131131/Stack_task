/**
 * @file
 * @brief Functions? that output smth to display like error message or stack dump
*/

#include <stdio.h>
#include <stdlib.h>

#include "Color_output.h"
#include "Stack.h"

enum errorCode stack_dump(FILE* stream, const struct Stack* stack, const char* file, const char* func, int line, stackDumpMode mode)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, file, func, line)) return NO_STACK_PTR;

    if (mode == FULL)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, stack->stackErrors);
        if (print_stack_homeland(stream, stack)) return NO_STACK_PTR;
    }

    if (stack_data_dump(stream, stack, mode)) return NO_STACK_PTR;

    return NO_ERRORS;
}

enum errorCode stack_data_dump(FILE* stream, const struct Stack* stack, stackDumpMode mode)
{
    if (no_ptr(stream, stack, NO_STACK_PTR, __FILE__, __func__, __LINE__)) return NO_STACK_PTR;

    if (mode == FULL)
    {
        #ifdef USE_CANARY_PROTECTION

        color_fprintf(stream, COLOR_PURPLE, STYLE_BOLD, "left canary");
        fprintf(stream, " = %llx\n", stack->leftCanary);

        color_fprintf(stream, COLOR_PURPLE, STYLE_BOLD, "right canary");
        fprintf(stream, " = %llx\n", stack->rightCanary);

        #endif
    }

    color_fprintf(stream, COLOR_PURPLE, STYLE_BOLD, "size");
    fprintf(stream, " = %lu\n", stack->size);

    color_fprintf(stream, COLOR_PURPLE, STYLE_BOLD, "capacity");
    fprintf(stream, " = %lu\n", stack->capacity);

    color_fprintf(stream, COLOR_PURPLE, STYLE_BOLD, "data");
    color_putc(stream, COLOR_BLUE, STYLE_BOLD, '[');
    if (!stack->data) 
    {
        color_fprintf(stream, COLOR_DEFAULT, STYLE_INVERT_C, "NULL");
        color_putc(stream, COLOR_BLUE, STYLE_BOLD, ']');
        fprintf(stream, "\n");
        return NO_STACK_DATA_PTR;
    }
    color_fprintf(stream, COLOR_DEFAULT, STYLE_INVERT_C, "%p", stack->data);
    color_putc(stream, COLOR_BLUE, STYLE_BOLD, ']');
    fprintf(stream, "\n");

    if (mode == FULL)
    {
        #ifdef USE_CANARY_PROTECTION

        color_fprintf(stream, COLOR_YELLOW, STYLE_BOLD, "left data canary");
        fprintf(stream, " = %llx\n", *((canary_t*) stack->data));

        #endif
    }

    size_t outputSize = 0;
    if (mode == FULL) outputSize = stack->capacity;
    else outputSize = stack->size;

    for (size_t i = 0; i < outputSize; i++)
    {
        if (i < stack->size) 
        {
            color_putc(stream, COLOR_CYAN, STYLE_BOLD, '*');
        }
        else if (i == stack->size)
        {
            color_putc(stream, COLOR_CYAN, STYLE_BOLD, '>');
        }
        else
            putc(' ', stream);
        color_putc(stream, COLOR_YELLOW, STYLE_BOLD, '[');
        fprintf(stream, "%lu", i);
        color_putc(stream, COLOR_YELLOW, STYLE_BOLD, ']');
        fprintf(stream, " = ");
        
        #ifdef USE_CANARY_PROTECTION

        if (((elem_t*) ((canary_t*) stack->data + 1))[i] == ELEM_T_POISON)
        {
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "POISON\n");
        }
        else
        {
            fprintf(stream, "%d\n", ((elem_t*) ((canary_t*) stack->data + 1))[i]);
        }

        #else

        if (stack->data[i] == ELEM_T_POISON)
        {
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "POISON\n");
        }
        else
        {
            fprintf(stream, "%d\n", stack->data[i]);
        }

        #endif
    }
    
    if (mode == FULL)
    {
        #ifdef USE_CANARY_PROTECTION

        color_fprintf(stream, COLOR_YELLOW, STYLE_BOLD, "right data canary");
        fprintf(stream, " = %llx\n", *((canary_t*) (((elem_t*) ((canary_t*) stack->data + 1)) + stack->capacity)));

        #endif
    }

    return NO_ERRORS;
}

void print_error(FILE* stream, enum errorCode error) //TODO assert
{
    #define PRINT_ERROR(error, errorCod, message) do{                   \
        if ((error) & (errorCod))                                       \
        {                                                               \
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");    \
            fprintf(stream, message);                                   \
        }                                                               \
    }while(0)

    PRINT_ERROR(error, NO_STACK_PTR,                        "Pointer to stack is NULL!\n");
    PRINT_ERROR(error, NO_STACK_DATA_PTR,                   "Pointer to stack data is NULL!\n");
    PRINT_ERROR(error, SIZE_OUT_OF_CAPACITY,                "Stack size more that stack capacity!\n");
    PRINT_ERROR(error, SIZE_NOT_VALID,                      "Stack size has a poison value(size invalid)!\n");
    PRINT_ERROR(error, CAPACITY_NOT_VALID,                  "Stack capacity has poison value(capacity invalid)!\n");
    PRINT_ERROR(error, NO_MEMORY,                           "Programm can't alloc the memory!\n");
    PRINT_ERROR(error, EMPTY_STACK,                         "Srack size is zero(stack is empty)!\n");
    PRINT_ERROR(error, LEFT_CANARY_BAD_VALUE,               "Left struct canary has a bad value!\n");
    PRINT_ERROR(error, RIGHT_CANARY_BAD_VALUE,              "Right struct canary has a bad value!\n");
    PRINT_ERROR(error, LEFT_DATA_CANARY_BAD_VALUE,          "Left data canary has a bad value!\n");
    PRINT_ERROR(error, RIGHT_DATA_CANARY_BAD_VALUE,         "Right data canary has a bad value!\n");
    PRINT_ERROR(error, BAD_STRUCT_HASH,                     "Bad struct hash!\n");
    PRINT_ERROR(error, BAD_DATA_HASH,                       "Bad data hash!\n");

    #undef PRINT_ERROR
}

enum errorCode no_ptr(FILE* stream, const void* ptr, enum errorCode error, const char* file, const char* func, int line)
{
    if (!ptr)
    {
        PRINT_LINE(stream, file, func, line);
        print_error(stream, error);
        return error;
    }

    return NO_ERRORS;
}

enum errorCode print_stack_homeland(FILE* stream, const struct Stack* stack)
{ 
    if (no_ptr(stream, stack, NO_STACK_PTR, __FILE__, __func__, __LINE__)) return NO_STACK_PTR;

    color_fprintf(stream, COLOR_BLUE, STYLE_BOLD, "Stack: ");

    color_putc(stream, COLOR_YELLOW, STYLE_BOLD, '[');
    color_fprintf(stream, COLOR_DEFAULT, STYLE_INVERT_C, "%p", stack);
    color_putc(stream, COLOR_YELLOW, STYLE_BOLD, ']');

    fprintf(stream, " \"%s\" initialised in file: ", stack->stackHomeland.stackName);
    
    color_fprintf(stream, COLOR_BLUE, STYLE_BOLD, "%s ", stack->stackHomeland.file);
    fprintf(stream, "function: ");
    color_fprintf(stream, COLOR_YELLOW, STYLE_BOLD, "%s(", stack->stackHomeland.function);
    color_fprintf(stream, COLOR_DEFAULT, STYLE_INVERT_C, "%d", stack->stackHomeland.line);
    color_fprintf(stream, COLOR_YELLOW, STYLE_BOLD, ")\n");

    return NO_ERRORS;
}