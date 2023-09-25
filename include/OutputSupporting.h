/**
 * @file 
 * @brief Supporting functions for output functions in Output.cpp
*/
#ifndef OUTPUT_SUPPORTING_H
#define OUTPUT_SUPPORTING_H

enum errorCode no_ptr(FILE* stream, const void* ptr, enum errorCode error, const char* file, const char* func, int line);

#endif