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

#ifndef _NEO_LIST_H
#define _NEO_LIST_H
#include <stdlib.h>

typedef void *npointer;
typedef const void *nconstpointer;

typedef struct _NeoList {
        /* hook up the data in any type */
        npointer data;

        /* linking of the previous node */
        struct _NeoList *prev;

        /* linking of the next node */
        struct _NeoList *next;
} NeoList;

/* a temporary variable to store literals or other variables */
void *__tmp_hook = NULL;

/*
 *
 * Here i recommend that use macro neo_data_to_npointer
 * to copy the value of a varible or a literal into
 * a block of addressable memory
 * so that you can save them into a linked list node or somewhere else
 *
 * */
#define neo_data_to_npointer( data, type ) \
        ( __tmp_hook = malloc ( sizeof ( type ) ), *(type *)__tmp_hook = ( data ), __tmp_hook )

/* read the data specified type using a pointer */
/* this macro matches up the macro neo_data_to_npointer */
#define neo_npointer_to_data( data, type ) *(type *)data

/*
 * list : the head pointer of a linked list
 * data : data specified by user (store in a node)
 * return value : return the head pointer of the list ( may be different from the initial value )
 * function : append a node
 * */
NeoList *neo_list_append ( NeoList *list, npointer data );

/*
 * list : the head pointer of a linked list
 * data : data specified by user (store in a node)
 * return value : return the head pointer of the list ( may be different from the initial value )
 * function : prepend a node
 */
NeoList *neo_list_prepend ( NeoList *list, npointer data );

/*
 * list : the head pointer of a linked list
 * data : data specified by user (store in a node)
 * position : the serial number of a node
 * return value : return the head pointer of the list ( may be different from the initial value )
 * function : insert a node before a position ( if position < 0 then add node to the tail)
 *
 * */
NeoList *neo_list_insert ( NeoList *list, npointer data, int position );

/*
 * list : the head pointer of a linked list
 * data : data specified by user (store in a node)
 * before : the node will be behind to adding node.
 * return value : return the head pointer of the list ( may be different from the initial value )
 * function : insert a node before a specified node
 *
 * */
NeoList *neo_list_insert_before ( NeoList *list, NeoList *before, npointer data );

/*
 * list : the head pointer of a linked list
 * data : data specified by user (store in a node)
 * NeoCompareFunc : a compare function offered by user
 * return value : return the head pointer of the list ( may be different from the initial value )
 * function : add a node according to the comparison result returned by the function NeoCompareFunc
 *
 */
NeoList *neo_list_insert_sorted ( NeoList *list, npointer data,
                                  int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ) );

/*
 * same as above but we will use user_data as a argument of the compare function
 * and you should free user_data if you allocted from the heap
 *
 * */
NeoList *neo_list_insert_sorted_with_data ( NeoList *list, npointer data,
                                            int ( *NeoCompareFunc ) ( nconstpointer a,
                                                                      nconstpointer b ),
                                            npointer user_data );

/*
 *
 * Removes an element from a NeoList. If two elements contain the same data, only the first is
 * removed.
 * If none of the elements contain the data, the NeoList is unchanged.
 * The data member of the node will not be free.
 * It will just free the node.
 * */
NeoList *neo_list_remove ( NeoList *list, npointer data,
                           int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ) );

/*
 * If you want to free the node totally
 * use the following function instead of the above
 *
 * */
NeoList *neo_list_remove_full ( NeoList *list, npointer data,
                                int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ),
                                void ( *NeoDestroyFunc ) ( npointer data ) );

/*
 * Remove the llink's link to its next element and previous element
 * Just the link !!!
 * */
NeoList *neo_list_remove_link ( NeoList *list, NeoList *llink );

/* remove link and destroy it */
NeoList *neo_list_delete_link ( NeoList *list, NeoList *link_,
                                void ( *NeoDestroyFunc ) ( npointer data ) );

/*
 * remove all the nodes if the data is the same as the node's data member
 * It will not free the data
 */
NeoList *neo_list_remove_all ( NeoList *list, npointer data,
                               int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ) );

/*
 * Free all node include its data and remove them if their data memeber is the same as user_data
 */
NeoList *neo_list_remove_all_full ( NeoList *list, npointer data,
                                    int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ),
                                    void ( *NeoDestroyFunc ) ( npointer data ) );

/*
 * sort the list
 */
NeoList *neo_list_sort ( NeoList *list,
                         int ( *NeoCompareFunc ) ( nconstpointer a, nconstpointer b ) );

/*
 * return the head of a list
 */
NeoList *neo_list_first ( NeoList *element );

/*
 * return the tail of a list
 * */
NeoList *neo_list_last ( NeoList *element );

/*
 * free all nodes but not the data
 *
 */
void neo_list_free ( NeoList *list );

/* free all nodes including their data member */
void neo_list_free_full ( NeoList *list, void ( *NeoDestroyFunc ) ( npointer data ) );

#endif /* neo_list.h */
