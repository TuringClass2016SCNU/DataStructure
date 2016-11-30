#ifndef _NMEMPOOL_H_
#define _NMEMPOOL_H_

#include "nallocator.h"
#include <stddef.h>

#define ALLOCATOR_TYPES 10
#define MIN_BLOCK_CNT ( size_t ) ( 1 << 10 )
#define MIN_BLOCK_SIZE ( size_t ) ( 1 << 4 )

typedef struct _NAllocatorHandle {
        /* An array that hold the fixed size NAllocator */

        /*
         *
         * Hold the memory whose size is between 16 bytes and 4096 btyes by default
         * size:
         * 	1:	size > 4096
         * 	>=2:	16 32 64 128 256 512 1024 2048 4096
         * 	total:	10
         *
         */
        char *owner;
        NAllocator **allocator_list;
        size_t min_blk_cnt;
        size_t min_blk_size;
        size_t allocator_types;
        size_t ( *increase_func ) ( size_t base_size );

        struct _NAllocatorHandle *prev;
        struct _NAllocatorHandle *next;
} NAllocatorHandle;

typedef struct _NMemoryPool {
        NAllocatorHandle *handle_list;
        NAllocatorHandle *active;
} NMemoryPool;

void n_mempool_destroy ();
void *n_mempool_alloc ( size_t mem_size );
void n_mempool_recycle ( void *ptr );
size_t n_mempool_increase_func ( size_t base_size );
void n_mempool_add_handler ( size_t min_blk_cnt, size_t min_blk_size, size_t allocator_types,
                             size_t ( *increase_func ) ( size_t base_size ), char *id );
size_t n_mempool_active ( char *id );
#endif /* end of include guard: nmempool.h */
