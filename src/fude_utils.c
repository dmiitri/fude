#include "fude.h"

#include <stdarg.h> // va_list
#include <stdio.h> // fprintf()
#include <stdlib.h> // exit(), EXIT_FAILURE

void* f_memcpy(void* dst, const void* src, size_t nbytes)
{
    for(size_t i = 0; i < nbytes; ++i)
        ((uint8_t*)dst)[i] = ((uint8_t*)src)[i];
    return dst;
}

void* f_memset(void* ptr, int value, size_t nbytes)
{
    for(size_t i = 0; i < nbytes; ++i)
        ((uint8_t*)ptr)[i] = (uint8_t)value;
    return ptr;
}

void* f_memzero(void* ptr, size_t nbytes)
{
    for(size_t i = 0; i < nbytes; ++i)
        ((uint8_t*)ptr)[i] = (uint8_t)0;
    return ptr;
}

void f_trace_log(int log_level, const char* fmt, ...)
{
    FILE* file = NULL;
    switch(log_level) {
    case FUDE_LOG_INFO:
        {
            file = stdout;
            fprintf(file, "[INFO] ");
        } break;
    case FUDE_LOG_WARNING:
        {
            fprintf(file, "[WARNING] ");
            file = stderr;
        } break;
    case FUDE_LOG_ERROR:
        {
            fprintf(file, "[ERROR] ");
            file = stderr;
        } break;
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
    fprintf(file, "\n"); 
}

void f_expect(bool condition, const char* fmt, ...)
{
    if(condition) return;

    fprintf(stderr, "[ERROR] ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n"); 
    exit(EXIT_FAILURE);
}

#if FUDE_PLATFORM_WINDOWS
#include <windows.h> // VirtualAllocEx(), MEM_COMMIT, MEM_RELEASE, MEM_RESERVE, 
                     // PAGE_READWRITE, GetCurrentProcess()

void* f_malloc(uint64_t nbytes)
{
    void* result = VirtualAllocEx(
            GetCurrentProcess(),
            NULL,
            nbytes,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE);
    return result;
}

void f_free(void* ptr)
{
    VirtualFreeEx(GetCurrentProcess(), (LPVOID)ptr, 0, MEM_RELEASE);
}

#endif
