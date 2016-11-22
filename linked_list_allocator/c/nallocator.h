#ifndef _NALLOCATOR_H_
#define _NALLOCATOR_H_

#include <stddef.h>

typedef struct _NBlock {

        struct _NBlock *prev;
        struct _NBlock *next;

	/* Convenient to quote allocator that contains the NBlock node */
	void *allocator_ref;

        void *data;
} NBlock;

typedef struct _NAllocator {

        /* Fundamental information */
        size_t blk_size;
        size_t blk_cnt;

        /* Maintain the following two list */
        /* the former is to record those Nblocks that are available */
        /* the latter is to record those Nblocks that are in used */
        NBlock *free_list;
        NBlock *busy_list;

        /* link to other NAllcators */
        struct _NAllocator *prev;
        struct _NAllocator *next;

} NAllocator;

NAllocator *n_allocator_init ( size_t blk_cnt, size_t blk_size );
NAllocator *n_allocator_append ( NAllocator *allocator );
void *n_allocator_alloc ( NAllocator *allocator );
void n_allocator_recycle (void *ptr);
#endif /* end of include guard: nallocator.h */
