#ifndef _SC_ALLOC_H_
#define _SC_ALLOC_H_
#define SC_MEMORY_META_SIZE sizeof(void *)

#define SC_MEMORY_SIZE_ 180
#define SC_MEMORY_SIZE CHAR_BIT * SC_MEMORY_SIZE_

#define SC_MEMORY_MINIMUM_MAPS_SIZE (SC_MEMORY_SIZE) / 2
#define SC_HEAP_MEMORY_SIZE SC_MEMORY_SIZE
#define SC_MEMORY_MINIMUM_SIZE CHAR_BIT
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
void* sc_minimum_alloc____(size_t size);
void* sc_maps_to_minimum_alloc____(size_t size);
void* sc_maps_alloc____(size_t size);
void* sc_alloc____(size_t size);
void* sc_free____(void* block);
#endif _SC_ALLOC_H_