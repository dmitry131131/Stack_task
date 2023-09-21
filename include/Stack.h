/**
 * @file
 * @brief Stack functions and structs
*/
#ifndef STACK_H
#define STACK_H
typedef int elem_t;
const elem_t ELEM_T_POISON = 0;

const size_t REALLOC_COEF        =  2;
const size_t SIZE_POISON_VAL     = -1;
const size_t CAPACITY_POISON_VAL = -1;

/// @brief Stack struct
struct Stack
{
    elem_t* data;       ///< Data array pointer
    size_t  size;       ///< Stack size(position of last element in array)
    size_t  capacity;   ///< Capacity of stack(max length of stack array)
};

/// @brief enum with stack error codes
enum errorCode
{
    NO_ERRORS,              ///< No errors(everything is Ok)
    NO_STACK_PTR,           ///< No stack pointer(passed NULL pointer to stack struct)
    NO_STACK_DATA_PTR,      ///< No data pointer(passed NULL pointer to stack data)
    SIZE_OUT_OF_CAPACITY,   ///< Size more than capacity(size >= capacity)
    SIZE_NOT_VALID,         ///< Size variable filled poison value 
    CAPACITY_NOT_VALID,     ///< Capacity variable filled poison value
    NO_MEMORY,              ///< Calloc function can't alloc the memory
    EMPTY_STACK             ///< Size is zero(empty stack)
};

#define CHECK_ERR(code)do{        \
    enum errorCode error = code;  \
    if (error)                    \
    {                             \
        return error;             \
    }                             \
}while(0)


/**
 * @brief Verification function for stack - check all stack data is valid
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_verify(struct Stack* stack);

/**
 * @brief Function initializes stack
 * @param [out] stack     Pointer to stack
 * @param [in]  capacity  Start capacity of stack  
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_ctor(struct Stack* stack, size_t capacity);

/**
 * @brief Function destruct stack
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_dtor(struct Stack* stack);

/**
 * @brief Function puts value into stack
 * @param [in] stack Pointer to stack
 * @param [in] value Value to push
 * @return Error code and NO_ERRORS if everythind ok
*/
enum errorCode stack_push(struct Stack* stack, elem_t value);

#endif