#include <stdio.h>

#include "Stack.h"

#ifdef USE_HASH_PROTECTION

hash_t jdb2_hash(const void* ptr, size_t objectSize)
{
    const char* pointer = (const char*) ptr;
    hash_t hash = 5381;

    unsigned c = 0;
    for (size_t i = 0; i < objectSize; i++)
    {
        c = (unsigned int) pointer[i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

enum errorCode calculate_hash(struct Stack* stack)
{
    if (no_ptr(stderr, stack, NO_STACK_PTR, __FILE__, __func__, __LINE__)) return NO_STACK_PTR;

    stack->structHash = 0;
    stack->dataHash   = 0;

    #ifdef USE_CANARY_PROTECTION

    size_t stackSize = sizeof(elem_t*) + 2*sizeof(size_t) + sizeof(errorCode) + 2*sizeof(hash_t) + sizeof(StackHomeland) +2*sizeof(canary_t);
    size_t dataSize  = stack->capacity*sizeof(elem_t) + 2*sizeof(canary_t);

    #else

    size_t stackSize = sizeof(elem_t*) + 2*sizeof(size_t) + sizeof(errorCode) + 2*sizeof(hash_t) + sizeof(StackHomeland);
    size_t dataSize  = stack->capacity*sizeof(elem_t);

    #endif

    hash_t structHashData = jdb2_hash(stack, stackSize);
    hash_t dataHashData   = jdb2_hash(stack->data, dataSize);

    stack->structHash = structHashData;
    stack->dataHash   = dataHashData;

    return NO_ERRORS;
}

#endif