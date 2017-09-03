#include "AVL.h"
using namespace std;

/**
 * Procedure: Transplant
 * TODO: get a node from a tree and move it to another place
 * param:
 * @T: AVLtree & : the tree
 * @u: node*     : dst node
 * @v: node*     : src node
 */
int AVL_Transplant(AVLtree &T, node *u, node *v) {
  if (u->p == T.nil) {
    T.root = v;
  } else if (u == u->p->left) {
    u->p->left = v;
  } else
    u->p->right = v;
  v->p = u->p;
  return 0;
}

/**
 * Procedure: AVL_Delete
 * @param T : AVLtree &
 * @param z : node *
 * @return  : int
 */

int AVL_Delete(AVLtree &T, node *z) {
    node *x;
    node* y = z;
    if(z->left == T.nil) {
        x = z->right;
        AVL_Transplant(T, z, z->right);
    }
}