/**
 * @file
 * @brief Stack functions and structs
*/
#ifndef STACK_H
#define STACK_H
typedef int elem_t;
const elem_t ELEM_T_POISON = 1;

const size_t REALLOC_COEF        = 2;
const size_t SIZE_POISON_VAL     = 18446744073709;
const size_t CAPACITY_POISON_VAL = 18446744073709;

/// @brief enum with stack error codes
enum errorCode
{
    NO_ERRORS             = 0,        ///< No errors(everything is Ok)
    NO_STACK_PTR          = 1 << 0,   ///< No stack pointer(passed NULL pointer to stack struct)
    NO_STACK_DATA_PTR     = 1 << 1,   ///< No data pointer(passed NULL pointer to stack data)
    SIZE_OUT_OF_CAPACITY  = 1 << 2,   ///< Size more than capacity(size >= capacity)
    SIZE_NOT_VALID        = 1 << 3,   ///< Size variable filled poison value 
    CAPACITY_NOT_VALID    = 1 << 4,   ///< Capacity variable filled poison value
    NO_MEMORY             = 1 << 5,   ///< Calloc function can't alloc the memory
    EMPTY_STACK           = 1 << 6    ///< Size is zero(empty stack)
};

/// @brief Struct with information about position where stack was initialised
struct StackHomeland
{
    const char* stackName;        ///< Name of stack variable
    const char* file;             ///< Name of file where stack was init
    const char* function;         ///< Name of function where struct was init
    int         line;             ///< Line of file where struvt was init
};

/// @brief Stack struct
struct Stack
{
    elem_t* data;                         ///< Data array pointer
    size_t  size;                         ///< Stack size(position of last element in array)
    size_t  capacity;                     ///< Capacity of stack(max length of stack array)

    enum errorCode stackErrors;           ///< Enum with all of stack errors

    struct StackHomeland stackHomeland;   ///< Struct with information about position where stack was initialised
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

#define STACK_DTOR(stack) stack_dtor((stack), stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_VERIFY(stack) stack_verify((stack), stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_CTOR(stack, capacity) do{                                                         \
                                                                                                \
    if(!stack_ctor((stack), capacity, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__))         \
    {                                                                                           \
        (stack)->stackHomeland = {#stack, __FILE__, __PRETTY_FUNCTION__, __LINE__};             \
    }                                                                                           \
                                                                                                \
}while(0)

#define STACK_PUSH(stack, value) stack_push((stack), value, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)
 
#define STACK_POP(stack) stack_pop((stack), stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

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
enum errorCode stack_realloc(struct Stack* stack, FILE* stream, const char* file, int line, const char* func);

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
elem_t stack_pop(struct Stack* stack, FILE* stream, const char* file, int line, const char* func);

/**
 * @brief Print all information about stack in stream
 * @param [in] stream Output stream
 * @param [in] stack  Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_data_dump(FILE* stream, const struct Stack* stack);

/**
 * @brief Function prints error description by errorCode in stream
 * @param [in] stream Output stream
 * @param [in] error  Error code
*/
void print_error(FILE* stream, enum errorCode error);

/**
 * @brief Function prints stack homeland with name of stack and place where it was initialised
 * @param [in] stream Output stream
 * @param [in] stack  Pointer to stack that homeland will be prined
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode print_stack_homeland(FILE* stream, const struct Stack* stack);

/**
 * @brief Function print stack homeland print all of errors in stack and dumps stack information
 * @param [in] stream Output stream
 * @param [in] stack  Pointer to stack
 * @param [in] file   File name 
 * @param [in] func   Function name
 * @param [in] line   Line number
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode stack_dump(FILE* stream, const struct Stack* stack, const char* file, const char* func, int line);

#endif