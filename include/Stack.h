/**
 * @file
 * @brief Stack functions and structs
*/
#ifndef STACK_H
#define STACK_H
typedef int elem_t;
const elem_t ELEM_T_POISON = 0;

const size_t REALLOC_COEF        = 2;
const size_t SIZE_POISON_VAL     = 18446744073709;
const size_t CAPACITY_POISON_VAL = 18446744073709;

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

#define PRINT_LINE(stream, file, func, line) do{                        \
    fprintf(stream, "In file: ");                                       \
    color_fprintf(stream, COLOR_CYAN, STYLE_UNDERLINED, "%s", file);    \
    fprintf(stream, ", function: ");                                    \
    color_fprintf(stream, COLOR_YELLOW, STYLE_BOLD, "%s", func);        \
    color_fprintf(stream, COLOR_BLUE, STYLE_BOLD, "(");                 \
    color_fprintf(stream, COLOR_DEFAULT, STYLE_INVERT_C, "%d", line);   \
    color_fprintf(stream, COLOR_BLUE, STYLE_BOLD, ")\n");               \
}while(0)

#define STACK_DTOR(stack) stack_dtor(stack, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_VERIFY(stack) stack_verify(stack, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_CTOR(stack, capacity) stack_ctor(stack, capacity, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_PUSH(stack, value) stack_push(stack, value, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)
 
#define STACK_PULL(stack) stack_pull(stack, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

/**
 * @brief Verification function for stack - check all stack data is valid
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_verify(struct Stack* stack, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Function initializes stack
 * @param [out] stack     Pointer to stack
 * @param [in]  capacity  Start capacity of stack  
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_ctor(struct Stack* stack, size_t capacity, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Function destruct stack
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_dtor(struct Stack* stack, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Function reallocs memory for struct data
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_realloc(struct Stack* stack);

/**
 * @brief Function puts value into stack
 * @param [in] stack Pointer to stack
 * @param [in] value Value to push
 * @return Error code and NO_ERRORS if everythind ok
*/
enum errorCode stack_push(struct Stack* stack, elem_t value, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Function pulls last element from stack 
 * @param [in] stack  Pointer to stack
 * @param [out] error Pointer to errorCode enum
 * @return Value of last element from stack
*/
elem_t stack_pull(struct Stack* stack, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Print all information about stack in stream
 * @param [in] stream Output stream
 * @param [in] stack  Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_dump(FILE* stream, const struct Stack* stack);

/**
 * @brief Function prints error description by errorCode in stream
 * @param [in] stream Output stream
 * @param [in] error  Error code
*/
void print_error(FILE* stream, enum errorCode error);

#endif