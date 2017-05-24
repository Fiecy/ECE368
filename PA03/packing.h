#include <stdio.h>
#include <stdlib.h>

typedef struct _TreeNode {
  char VH;
  int value;
  struct _TreeNode* left;
  struct _TreeNode* right;
  struct _TreeNode* up;
  struct _TreeNode* down;
  double width;
  double height;
  double x;
  double y;
} TreeNode;

void push (TreeNode** stack, TreeNode* Node);
TreeNode* pop (TreeNode** stack);
TreeNode* Load_From_File(char* Filename);
void Free_Tree (TreeNode* root);
void Perform_Packing(TreeNode* root, double* width, double* height, double* x, double* y, int* value);
void Save_Packing(char* Filename, TreeNode* root);
