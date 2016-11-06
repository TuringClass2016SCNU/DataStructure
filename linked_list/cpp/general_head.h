#ifndef HEAD_H_
#define HEAD_H_
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#endif

#ifndef _LINK_LIST_
#define _LINK_LIST_
template <class T> class node {
public:
  T value;
  node *next, *prev;
};
template <class T> class link_list : public node<T> {
public:
  link_list() {}
  ~link_list();
  void create();
  void add(int count = 1);
  void rm(int count = 1);
  void insert(node<T>);
  void swap(node<class T>, node<class T>)

      private : void destroy();
  node<T> *head;
  node<T> *current;
};
#endif
