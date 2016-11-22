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
#include <stdlib.h>

#define ALIGN_SIZE 16
#define n_type_malloc( type, count ) (type *)malloc ( sizeof ( type ) * count )
#define n_size_malloc( size ) malloc ( ( size ) )
#define n_alignment_up( oper, size ) ( ( ( oper ) + (size)-1 ) & ( ~( (size)-1 ) ) )

// static NAllocator *last_allocator = NULL;

NAllocator *allocator_list = NULL;

static void n_allocator_map_mem ( NAllocator *allocator, void *real_mem );

NAllocator *n_allocator_init ( size_t blk_cnt, size_t blk_size ) {

        /* Allocator initializaiton */
        NAllocator *allocator = n_type_malloc ( NAllocator, 1 );

        allocator->blk_cnt = blk_cnt;
        allocator->blk_size = n_alignment_up ( sizeof ( NBlock * ), ALIGN_SIZE ) +
                              n_alignment_up ( blk_size, ALIGN_SIZE );
        allocator->prev = allocator->next = NULL;

        n_allocator_map_mem ( allocator, malloc ( allocator->blk_cnt * allocator->blk_size ) );
        return allocator;
}

static void n_allocator_map_mem ( NAllocator *allocator, void *real_mem ) {

        NBlock *nblocks = n_type_malloc ( NBlock, allocator->blk_cnt );
        allocator->free_list = nblocks;
        allocator->busy_list = NULL;

        NBlock *iter = nblocks, *bound = iter + allocator->blk_cnt;
        char *tmp_ptr = real_mem;

        iter->prev = NULL;
        while ( iter < bound ) {
                iter->allocator_ref = allocator;
                iter->data = tmp_ptr + n_alignment_up ( sizeof ( NBlock * ), ALIGN_SIZE );
                iter->next = ( iter + 1 != bound ) ? iter + 1 : NULL;

                if ( iter->next != NULL )
                        iter->next->prev = iter;

                *(NBlock **)tmp_ptr = iter;
                tmp_ptr += allocator->blk_size;
                ++iter;
        }
}

NAllocator *n_allocator_append ( NAllocator *allocator ) {
        /* If allocator is NULL, do not add anything */
        if ( allocator == NULL )
                return allocator_list;

        /* If allcator list is empty, add it as the head */
        if ( allocator_list == NULL )
                allocator_list = allocator;

        else {
                /* Add on to the tail */
                allocator_list->next = allocator;
                allocator->prev = allocator_list;
                allocator_list = allocator;
        }

        return allocator_list;
}

void *n_allocator_alloc ( NAllocator *allocator ) {

        /* Allocate failed */
        if ( allocator->free_list == NULL )
                return NULL;
        /* Record the element that will be allocated */
        NBlock *allocated = allocator->free_list;

        if ( allocator->busy_list == NULL )
                allocator->busy_list = allocated;
        else {
                /* Add it on to the end */
                allocator->busy_list->next = allocated;
                allocated->prev = allocator->busy_list;
        }
        allocator->free_list = allocated->next;

        if ( allocator->next )
                allocator->next->prev = NULL;

        return allocated->data;
}

void n_allocator_recycle ( void *ptr ) {
        if ( ptr == NULL )
                return;
        NBlock *release =
            *(NBlock **)( (char *)ptr - n_alignment_up ( sizeof ( NBlock * ), ALIGN_SIZE ) );
        NAllocator *allocator = release->allocator_ref;

        if ( release->prev )
                release->prev->next = release->next;
        if ( release->next )
                release->next->prev = release->prev;


        allocator->busy_list = release->next;

        release->prev = release->next = NULL;

        if ( allocator->free_list == NULL )
                allocator->free_list = release;
        else {
                allocator->free_list->prev = release;
                release->next = allocator->free_list;
        }
}
