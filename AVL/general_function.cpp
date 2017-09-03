#include "AVL.h"

node *getMinPointer(AVLtree &T, node *x) {
  node *t = x;
  while (t->left != T.nil) {
    t = t->left;
  }
  return t;
}

int getMinKey(AVLtree &T, node *x) {
  node *t = x;
  while (t->left != T.nil) {
    t = t->left;
  }
  return t->key;
}

node *getMaxPointer(AVLtree &T, node *x) {
  node *t = x;
  while (t->right != T.nil) {
    t = t->right;
  }
  return t;
}

node *getMaxKey(AVLtree &T, node *x) {
  node *t = x;
  while (t->right != T.nil) {
    t = t->right;
  }
  return t;
}
