/**
 * @file
 * @brief Stack functions and structs
*/
#ifndef STACK_H
#define STACK_H

typedef int elem_t;
const elem_t ELEM_T_POISON = 2147483647;

#define USE_CANARY_PROTECTION
#define USE_HASH_PROTECTION

#ifdef USE_CANARY_PROTECTION

typedef unsigned long long canary_t;
const canary_t CANARY_T_DEFAULT = 0xBADC0FFEE;
const canary_t CANARY_T_POISON  = 18446744073709;

#endif

#ifdef USE_HASH_PROTECTION

typedef unsigned long long hash_t;

#endif

const size_t REALLOC_COEF        = 2;
const size_t SIZE_POISON_VAL     = 18446744073709;
const size_t CAPACITY_POISON_VAL = 18446744073709;

/// @brief enum with stack error codes
enum errorCode
{
    NO_ERRORS                       = 0,        ///< No errors(everything is Ok)
    NO_STACK_PTR                    = 1 << 0,   ///< No stack pointer(passed NULL pointer to stack struct)
    NO_STACK_DATA_PTR               = 1 << 1,   ///< No data pointer(passed NULL pointer to stack data)
    SIZE_OUT_OF_CAPACITY            = 1 << 2,   ///< Size more than capacity(size >= capacity)
    SIZE_NOT_VALID                  = 1 << 3,   ///< Size variable filled poison value 
    CAPACITY_NOT_VALID              = 1 << 4,   ///< Capacity variable filled poison value
    NO_MEMORY                       = 1 << 5,   ///< Calloc function can't alloc the memory
    EMPTY_STACK                     = 1 << 6,   ///< Size is zero(empty stack)
    LEFT_CANARY_BAD_VALUE           = 1 << 7,   ///< Bad value of left canary
    RIGHT_CANARY_BAD_VALUE          = 1 << 8,   ///< Bad value of right canary
    LEFT_DATA_CANARY_BAD_VALUE      = 1 << 9,   ///< Bad value of right canary
    RIGHT_DATA_CANARY_BAD_VALUE     = 1 << 10,  ///< Bad value of right canary
    BAD_STRUCT_HASH                 = 1 << 11,  ///< Bad struct hash
    BAD_DATA_HASH                   = 1 << 12   ///< Bad data hash
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
    #ifdef USE_CANARY_PROTECTION
    canary_t leftCanary;                  ///< Left protection canary
    #endif

    elem_t* data;                         ///< Data array pointer
    size_t  size;                         ///< Stack size(position of last element in array)
    size_t  capacity;                     ///< Capacity of stack(max length of stack array)

    enum errorCode stackErrors;           ///< Enum with all of stack errors

    #ifdef USE_HASH_PROTECTION
    hash_t structHash;
    hash_t dataHash;
    #endif

    struct StackHomeland stackHomeland;   ///< Struct with information about position where stack was initialised

    #ifdef USE_CANARY_PROTECTION
    canary_t rightCanary;                 ///< Right protection canary
    #endif
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
    if(!no_ptr(stderr, (stack), NO_STACK_PTR, __FILE__, __PRETTY_FUNCTION__, __LINE__))         \
    {                                                                                           \
        (stack)->stackHomeland = {#stack, __FILE__, __PRETTY_FUNCTION__, __LINE__};             \
        stack_ctor((stack), capacity, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__);         \
    }                                                                                           \
    else print_error(stderr, NO_STACK_PTR);                                                     \
                                                                                                \
}while(0)

#define STACK_PUSH(stack, value) stack_push((stack), value, stderr, __FILE__, __LINE__, __PRETTY_FUNCTION__)
 
#define STACK_POP(stack) stack_pop((stack), stdout, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define STACK_DUMP(stack) stack_dump(stdout, (stack), __FILE__, __PRETTY_FUNCTION__, __LINE__)

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

/**
 * @brief Function checks that pointer isn't null
 * @param [in] stream Output stream
 * @param [in] ptr    Pointer to object
 * @param [in] file   File name 
 * @param [in] func   Function name
 * @param [in] line   Line number
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode no_ptr(FILE* stream, const void* ptr, enum errorCode error, const char* file, const char* func, int line);

/**
 * @brief Function testing all struct functions
 * @param [in] steram Message output stream
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode test_driver(FILE* stream);

#ifdef USE_HASH_PROTECTION

/**
 * @brief Function calculating jdb2 hash 
 * @param [in] ptr        Pointer to object that need to hash
 * @param [in] objectSize Object size in bytes
 * @return Hash or 0 if error in calculating
*/
hash_t jdb2_hash(const void* ptr, size_t objectSize);

#endif

/**
 * @brief Function recalculate hash of stack
 * @param [in] stack Pointer to stack
 * @return Error code or NO_ERRORS if everything ok
*/
enum errorCode calculate_hash(struct Stack* stack);

#endif