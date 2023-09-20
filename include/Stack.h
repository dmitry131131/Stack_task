/**
 * @file
 * @brief Stack functions and structs
*/
#ifndef STACK_H
#define STACK_H
typedef int elem_t;

struct Stack
{
    elem_t* data;
    size_t  size;
    size_t  capacity;
};

enum errorCode
{
    NO_ERRORS,
    NO_STACK_PTR,
    NO_STACK_DATA_PTR,
    SIZE_OUT_OF_CAPACITY,
    SIZE_NOT_VALID,
    CAPACITY_NOT_VALID
};


#endif