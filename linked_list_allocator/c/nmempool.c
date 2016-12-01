/*
 * Copyright (c) 2016 Deng, Liming <myosmanthustree@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*

    █████▒█    ██  ▄████▄   ██ ▄█▀       ██████╗ ██╗   ██╗ ██████╗
  ▓██   ▒ ██  ▓██▒▒██▀ ▀█   ██▄█▒        ██╔══██╗██║   ██║██╔════╝
  ▒████ ░▓██  ▒██░▒▓█    ▄ ▓███▄░        ██████╔╝██║   ██║██║  ███╗
  ░▓█▒  ░▓▓█  ░██░▒▓▓▄ ▄██▒▓██ █▄        ██╔══██╗██║   ██║██║   ██║
  ░▒█░   ▒▒█████▓ ▒ ▓███▀ ░▒██▒ █▄       ██████╔╝╚██████╔╝╚██████╔╝
   ▒ ░   ░▒▓▒ ▒ ▒ ░ ░▒ ▒  ░▒ ▒▒ ▓▒       ╚═════╝  ╚═════╝  ╚═════╝
   ░     ░░▒░ ░ ░   ░  ▒   ░ ░▒ ▒░
   ░ ░    ░░░ ░ ░ ░        ░ ░░ ░
            ░     ░ ░      ░  ░
*/

#include "nallocator.h"
#include "nmempool.h"
#include <stdlib.h>
#include <string.h>

typedef struct _NAllocatorHandle {
        /* An array that hold the fixed size NAllocator */

        /*
         *
         * Hold the memory whose size is between 16 bytes and 4096 btyes by
         * default
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

static NMemoryPool *n_global_pool = NULL;
static NAllocatorHandle *n_mempool_handler_init ( size_t min_blk_cnt, size_t min_blk_size,
                                                  size_t allocator_types,
                                                  size_t ( *increase_func ) ( size_t base_size ),
                                                  char *id );

size_t n_mempool_increase_func ( size_t base_size ) { return base_size << 1; }

void *n_mempool_alloc ( size_t mem_size ) {
        if ( n_global_pool == NULL )
                n_mempool_add_handler ( MIN_BLOCK_CNT, MIN_BLOCK_SIZE, ALLOCATOR_TYPES,
                                        n_mempool_increase_func, NULL );

        mem_size = n_alignment_up ( mem_size, ALIGN_SIZE ) +
                   n_alignment_up ( sizeof ( NBlock * ), ALIGN_SIZE );

        NAllocator **p_allocator = NULL, **bound = NULL;
        for ( p_allocator = n_global_pool->active->allocator_list,
              bound = p_allocator + n_global_pool->active->allocator_types;
              p_allocator != bound - 1 && mem_size > p_allocator[ 0 ]->blk_size; ++p_allocator )
                ;

        if ( p_allocator == bound - 1 ) {
                NAllocator *t = n_allocator_init ( 1, mem_size );
                if ( p_allocator[ 0 ] == NULL )
                        p_allocator[ 0 ] = t;
                else {
                        p_allocator[ 0 ]->next = t;
                        t->prev = p_allocator[ 0 ];
                }
                return n_allocator_alloc ( t );
        }
        NAllocator *iter = p_allocator[ 0 ], *prev = NULL;
        while ( iter && iter->free_list == NULL ) {
                if ( iter->next == NULL )
                        prev = iter;
                iter = iter->next;
        }
        if ( iter == NULL ) {
                NAllocator *t =
                    n_allocator_init ( p_allocator[ 0 ]->blk_cnt, p_allocator[ 0 ]->blk_size );
                prev->next = t;
                t->prev = prev;
                return n_allocator_alloc ( t );
        }
        return n_allocator_alloc ( iter );
}

void n_mempool_recycle ( void *ptr ) { n_allocator_recycle ( ptr ); }

void n_mempool_destroy () {
        if ( n_global_pool == NULL )
                return;
        NAllocatorHandle *handler = n_global_pool->handle_list;
        while ( handler ) {

                NAllocatorHandle *t_handler = handler->next;

                NAllocator **p_allocator = NULL, **bound = NULL;

                for ( p_allocator = handler->allocator_list,
                      bound = p_allocator + handler->allocator_types;
                      p_allocator != bound; ++p_allocator ) {
                        NAllocator *allocator = p_allocator[ 0 ];
                        while ( allocator ) {
                                NAllocator *t_allocator = allocator->next;
                                free ( allocator->abstract_mem );
                                free ( allocator->real_mem );
                                free ( allocator );
                                allocator = t_allocator;
                        }
                }

                free ( handler->allocator_list );
                free ( handler );
                handler = t_handler;
        }
        free ( n_global_pool );
}

void n_mempool_add_handler ( size_t min_blk_cnt, size_t min_blk_size, size_t allocator_types,
                             size_t ( *increase_func ) ( size_t base_size ), char *id ) {

        NAllocatorHandle *new = n_mempool_handler_init ( min_blk_cnt, min_blk_size, allocator_types,
                                                         increase_func, id );
        if ( n_global_pool == NULL ) {
                n_global_pool = n_type_malloc ( NMemoryPool, 1 );
                n_global_pool->handle_list = n_global_pool->active = new;
        } else {
                NAllocatorHandle *iter = n_global_pool->handle_list;
                while ( iter->next )
                        iter = iter->next;
                iter->next = new;
                new->prev = iter;
        }
}

static NAllocatorHandle *n_mempool_handler_init ( size_t min_blk_cnt, size_t min_blk_size,
                                                  size_t allocator_types,
                                                  size_t ( *increase_func ) ( size_t base_size ),
                                                  char *id ) {
        NAllocatorHandle *new = n_type_malloc ( NAllocatorHandle, 1 );
        new->owner = id;
        new->prev = new->next = NULL;
        new->min_blk_cnt = min_blk_cnt;
        new->min_blk_size = min_blk_size;
        new->allocator_types = allocator_types;
        new->increase_func = increase_func;

        new->allocator_list = n_type_malloc ( NAllocator *, allocator_types );

        NAllocator **iter = new->allocator_list, **bound = iter + allocator_types;
        size_t bsize = min_blk_size;
        while ( iter < bound - 1 ) {
                iter[ 0 ] = n_allocator_init ( min_blk_cnt, bsize );
                iter[ 0 ]->prev = iter[ 0 ]->next = NULL;
                bsize = increase_func ( bsize );
                ++iter;
        }
        *iter = NULL;

        return new;
}

size_t n_mempool_active ( char *id ) {
        if ( id == NULL )
                return 0;
        NAllocatorHandle *iter = n_global_pool->handle_list;
        while ( iter && iter->owner == NULL )
                iter = iter->next;
        while ( iter ) {
                if ( strcmp ( id, iter->owner ) == 0 )
                        break;
                iter = iter->next;
        }
        if ( iter ) {
                n_global_pool->active = iter;
                return 1;
        }
        return 0;
}
