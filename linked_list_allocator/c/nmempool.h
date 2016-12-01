#ifndef _NMEMPOOL_H_
#define _NMEMPOOL_H_

#include <stddef.h>

#define ALLOCATOR_TYPES 10
#define MIN_BLOCK_CNT ( size_t ) ( 1 << 10 )
#define MIN_BLOCK_SIZE ( size_t ) ( 1 << 4 )

/* Allocate a block of memory from the actived memory pool */
void *n_mempool_alloc ( size_t mem_size );

/* Recycle a memory block but not free it */
void n_mempool_recycle ( void *ptr );

/* Free the memory pool */
void n_mempool_destroy ();

/* Active a pool by specifying a memory pool identity */
size_t n_mempool_active ( char *id );

/* The default mempool allcator increase_func */
size_t n_mempool_increase_func ( size_t base_size );

/* You can add a custom memory pool and it will be actived automatically */
void n_mempool_add_handler ( size_t min_blk_cnt, size_t min_blk_size, size_t allocator_types,
                             size_t ( *increase_func ) ( size_t base_size ), char *id );

#endif /* end of include guard: nmempool.h */
