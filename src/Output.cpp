/**
 * @file
 * @brief Functions? that output smth to display like error message or stack dump
*/

#include <stdio.h>
#include <stdlib.h>

#include "Color_output.h"
#include "Stack.h"

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

void print_error(FILE* stream, enum errorCode error) //TODO assert
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