#include <stdio.h>
#include <stdlib.h>
#include "packing.h"

/* A print function to check the tree
void _print_tree(TreeNode* root){
  if (root->left != NULL) {
    printf("go left\n");
    _print_tree(root->left);
  }
  if (root->right != NULL) {
    printf("go right\n");
    _print_tree(root->right);
  }
  if (root->value != -1) {
      printf("the root: w is %le, h is %le\n", root->width, root->height);
  }
  else {
    printf("%c\n", root->VH);
  }
  printf("go back\n");
}
*/
int main(int argc, char *argv[]){
  TreeNode* root = Load_From_File(argv[1]);
  
  
  double width = 0;
  double height = 0;
  double x = 0;
  double y = 0;
  int value = 0;
  Perform_Packing(root, &width, &height, &x, &y, &value);
  
  /////////////// PA 04 ////////////////////
  double best_w = root->width;
  double best_h = root->height;
  
  if (root->left != NULL) {
      Re_Rooting(root, root->left, &best_w, &best_h);
      Re_Rooting(root, root->right, &best_w, &best_h);
  }
  ///////////////////////////////////////////

  printf("Width: %le\nHeight: %le\n", root->width, root->height);
  printf("X-coordinate: %le\nY-coordinate: %le\n", x, y);
  printf("Best width: %le\nBest height: %le\n", best_w, best_h);
  //_print_tree(root);
  Save_Packing(argv[2], root);
  Free_Tree(root);
  return 0;
}
