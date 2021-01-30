#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define M_CHUNK getpagesize()

#define HEADER_S sizeof(size_t)
#define HEADER_C (sizeof(size_t) * 2)

#define ALIGN HEADER_S
#define PADDING(mem_asked) ((ALIGN - (mem_asked % ALIGN)) % ALIGN)

#define FLAG_FREE 0x1
#define FLAG_LAST_HEADER 0x1

#define IS_PREV_FREE(x) (x & FLAG_FREE)
#define IS_LAST_HEADER(x) (x & FLAG_LAST_HEADER)

#define SIZET_P(x) ((size_t *) x)
#define CHAR_P(x) ((char *) x)
#define VOID_P(x) ((void *) x)

#define GET_PREV_SIZE(x) (SIZET_P(x)[0])
#define GET_CURR_SIZE(x) (SIZET_P(x)[1])

#define PREV_SIZE(x) (SIZET_P(x)[0])
#define CURR_SIZE(x) (SIZET_P(x)[1])

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t size);
#endif
