/*
 * Copyright (c) 2016 Deng, Liming <myosmanthustree@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of
 * the Software, and to permit persons to whom the Software is furnished to do
 * so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER
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
                    ░
*/

#include "nallocator.h"
#include <stdio.h>

int main ( void ) {
        NAllocator *allocator = n_allocator_init ( 4, 8 );
        size_t *a[ 5 ];
        for ( size_t i = 0; i < 4; ++i )
                a[ i ] = n_allocator_alloc ( allocator ), *a[ i ] = i + 3;
        for ( size_t i = 0; i < 4; ++i )
                printf ( "%ld\n", *a[ i ] );
        n_allocator_recycle ( a[ 3 ] );
        a[ 4 ] = n_allocator_alloc ( allocator );
        printf ( "%p\n", a[ 4 ] );
        return 0;
}
