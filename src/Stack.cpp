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

    if (stack->size     == SIZE_POISON_VAL)
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

elem_t stack_pull(struct Stack* stack, FILE* stream, const char* file, int line, const char* func)
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

enum errorCode stack_dump(FILE* stream, const struct Stack* stack)
{
    if (!stack) return NO_STACK_PTR;
    
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
    for (size_t i = 0; i < stack->capacity; i++)
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
        if (stack->data[i] == ELEM_T_POISON)
        {
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "POISON\n");
        }
        else
        {
            fprintf(stream, "%d\n", stack->data[i]);
        }
    }
    return NO_ERRORS;
}

void print_error(FILE* stream, enum errorCode error)
{
    color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Error: ");
    switch (error)
    {
    case NO_ERRORS:
        break;
    
    case NO_STACK_PTR:
        fprintf(stream, "Pointer to stack is NULL!\n");
        break;
    
    case NO_STACK_DATA_PTR:
        fprintf(stream, "Pointer to stack data is NULL!\n");
        break;

    case SIZE_OUT_OF_CAPACITY:
        fprintf(stream, "Stack size more that stack capacity!\n");
        break;

    case SIZE_NOT_VALID:
        fprintf(stream, "Stack size has a poison value(size invalid)!\n");
        break;

    case CAPACITY_NOT_VALID:
        fprintf(stream, "Stack capacity has poison value(capacity invalid)!\n");
        break;

    case NO_MEMORY:
        fprintf(stream, "Programm can't alloc the memory!\n");
        break;

    case EMPTY_STACK:
        fprintf(stream, "Srack size is zero(stack is empty)!\n");
        break;

    default:
        break;
    }
}
