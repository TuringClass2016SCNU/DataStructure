#ifndef _NALLOCATOR_H_
#define _NALLOCATOR_H_

#define ALIGN_SIZE 16
#define MAGIC_NUMNER (size_t)0x061BEB632DD5E318

#define n_type_malloc( type, count ) (type *)malloc ( sizeof ( type ) * count )
#define n_size_malloc( size ) malloc ( ( size ) )
#define n_alignment_up( oper, size ) ( ( ( oper ) + (size)-1 ) & ~( (size)-1 ) )

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

        /* To free memory more conveniently */
        void *real_mem;
        void *abstract_mem;

        void *handle_ref;

        /* link to other NAllcators */
        struct _NAllocator *prev;
        struct _NAllocator *next;

} NAllocator;

NAllocator *n_allocator_init ( size_t blk_cnt, size_t blk_size );

void *n_allocator_alloc ( NAllocator *allocator );

void n_allocator_recycle ( void *ptr );

#endif /* end of include guard: nallocator.h */
