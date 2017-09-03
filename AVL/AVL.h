#ifndef __HEAD__
#define __HEAD__
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
struct node {
  int key;
  int avlK;
  node *left, *right, *p;
  node() : key(0), avlK(0), left(NULL), right(NULL), p(NULL) {}
  node(int key, int level = 0)
      : key(0), avlK(level), left(NULL), right(NULL), p(NULL) {}
};

struct AVLtree {
  node *root;
  node *nil;
  AVLtree() {
    nil = new node(-1, -1);
    root = nil;
    nil->right = nil;
    nil->left = nil;
  }
};
#endif
