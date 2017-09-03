#include "AVL.h"
int ag;

int inOrder(AVLtree &T, node *x) {
  if (x != T.nil) {
    inOrder(T, x->left);
    printf("%d : %d\n", x->key, x->avlK);
    inOrder(T, x->right);
  }
  return 1;
}
// Original rotate method.
int leftRotate(AVLtree &T, node *x) {
  node *y = x->right;
  x->right = y->left;
  if (y->left != T.nil) {
    y->left->p = x;
  }
  y->p = x->p;

  if (x->p == T.nil) {
    T.root = y;
  } else if (x == x->p->left) {
    x->p->left = y;
  } else {
    x->p->right = y;
  }
  y->left = x;
  x->p = y;
  return 1;
}

// 验证正确
int rightRotate(AVLtree &T, node *x) {
  node *y = x->left;
  x->left = y->right;
  if (y->right != T.nil) {
    y->right->p = x;
  }
  y->p = x->p;

  if (x->p == T.nil) {
    T.root = y;
  } else if (x == x->p->right) {
    x->p->right = y;
  } else {
    x->p->left = y;
  }
  y->right = x;
  x->p = y;
  return 1;
}
int insertFix(AVLtree &T, node *x) {
  // 务必要小心
  bool Flag = 0;
  node *z = NULL;
  int highTmp = 0;
  node *y = NULL;
  while (x != T.root) {
    Flag = 0;
    y = x->p;
    highTmp++;
    if (highTmp > y->avlK) {
      y->avlK = highTmp;
    }
    if (y->left->avlK - y->right->avlK >= 2) {
      Flag = 1;
      if (x->right->avlK > x->left->avlK) {
        leftRotate(T, x);
        x->avlK = x->left->avlK > x->right->avlK ? x->left->avlK + 1
                                                 : x->right->avlK + 1;
        y->avlK = y->left->avlK > y->right->avlK ? y->left->avlK + 1
                                                 : y->right->avlK + 1;
        x = y->left;
      }
      rightRotate(T, y);
      y->avlK = y->left->avlK > y->right->avlK ? y->left->avlK + 1
                                               : y->right->avlK + 1;
      x->avlK = x->left->avlK > x->right->avlK ? x->left->avlK + 1
                                               : x->right->avlK + 1;
      // DO Sth
    } else if (y->right->avlK - y->left->avlK >= 2) {
      Flag = 1;
      if (x->left->avlK > x->right->avlK) {
        rightRotate(T, x);
        x->avlK = x->left->avlK > x->right->avlK ? x->left->avlK + 1
                                                 : x->right->avlK + 1;
        y->avlK = y->left->avlK > y->right->avlK ? y->left->avlK + 1
                                                 : y->right->avlK + 1;
        x = y->right;
      }
      leftRotate(T, y);
      y->avlK = y->right->avlK > y->left->avlK ? y->right->avlK + 1
                                               : y->left->avlK + 1;
      x->avlK = x->left->avlK > x->right->avlK ? x->left->avlK + 1
                                               : x->right->avlK + 1;
    }
    highTmp = Flag ? x->avlK : highTmp;
    if (x == T.root) {
      break;
    }
    x = x->p;
  }
  return 1;
}

int First(AVLtree &first) {
  if (first.root == NULL)
    first.root = new node;
  return 0;
  // Didn't finished
}

/**
 * Procedure Insert:
 * TODO: insert the target node into one tree.
 * param:
 * T: the target tree
 * ins: the node to be inserted
 */
int Insert(AVLtree &T, node *ins) {
  // y: trace x's parent;
  // x: trace y's parent;
  node *x, *y;

  // initalize x, y
  y = T.nil;
  x = T.root;
  while (x != T.nil) { // trace the pos
    y = x;
    if (ins->key < x->key) {
      x = x->left;
    } else
      x = x->right;
  }                 // Go as routinue;
  ins->p = y;       // Bind its parent
  if (y == T.nil) { // Case of only one node
    T.root = ins;
    y = T.root;
    T.root->p = T.nil;
  } else if (ins->key < y->key) {
    y->left = ins;
  } else {
    y->right = ins;
  }
  ins->left = T.nil;
  ins->right = T.nil;
  insertFix(T, ins);
  return 0; // Status code. Just persional convention.
}

int main(int argc, char const *argv[]) {
  AVLtree T;
  node *ins = new node[12];
  ins[0].key = 76;
  ins[1].key = 54;
  ins[2].key = 32;
  ins[3].key = 16;
  ins[4].key = 14;
  ins[5].key = 10;
  ins[6].key = 18;
  ins[7].key = 26;
  ins[8].key = 30;
  ins[9].key = 28;
  ins[10].key = 12;
  ins[11].key = 6;
  for (ag = 0; ag <= 10; ag++) {
    Insert(T, &ins[ag]);
    inOrder(T, T.root);
    printf("------------------\n");
  }

  return 0;
}
