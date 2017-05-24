#include <stdio.h>
#include <stdlib.h>
#include "packing.h"

/*
void _FIFP(TreeNode* stack){
  if (stack == NULL){
	printf("stack is NULL\n");
	return;
  }

  if (stack->left == NULL){
	printf("%d\n", stack->value);
	return;
  }
  
  _FIFP(stack->left);
  printf("%d\n", stack->value);
}
*/

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

int main(int argc, char *argv[]){
  TreeNode* root = Load_From_File(argv[1]);

  
  double width = 0;
  double height = 0;
  double x = 0;
  double y = 0;
  int value = 0;
  Perform_Packing(root, &width, &height, &x, &y, &value);

  printf("Width: %le\nHeight: %le\n", root->width, root->height);
  printf("X-coordinate: %le\nY-coordinate: %le\n", x, y);
  //_print_tree(root);
  Save_Packing(argv[2], root);
  Free_Tree(root);
  return 0;
}
