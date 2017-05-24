#include <stdio.h>
#include <stdlib.h>
#include "packing.h"

void push(TreeNode** stack, TreeNode* Node) {
	if (*stack == NULL) {
		Node->down = NULL;
		Node->up = NULL;
		*stack = Node;
		return;
	}
	else {
	    (*stack)->down = Node;
		Node->up = *stack;
		Node->down = NULL;
		*stack = Node;
		return;
	}
}

TreeNode* pop(TreeNode** stack) {
	TreeNode* Node = NULL;
	if (*stack != NULL) {
		Node = *stack;
		*stack = (*stack) -> up;
		if (*stack != NULL) {
		  (*stack) -> down = NULL;
		}
		Node -> up = NULL;
		return Node;
	}
	printf("pop from an empty Stack\n");
	return Node;
}

void Free_Tree(TreeNode* root) {
    if (root != NULL) {
	  if (root->left != NULL) {
		Free_Tree(root->left);
	  }
	  if (root->right != NULL) {
		Free_Tree(root->right);
	  }
	  free(root);
	}
}

TreeNode* Load_From_File(char* Filename) {
  TreeNode* Node = NULL;
  TreeNode* stack = NULL;
  FILE* ptr = fopen(Filename, "r");
  int test = 1;
  
  while(test > 0) {
	Node = malloc(sizeof(TreeNode));
	Node->left = NULL;
	Node->right = NULL;
	//test = fscanf(ptr, "%c", &(Node->value));
	test = fscanf(ptr, "%d(%le,%le)\n", &(Node->value), &(Node->width), &(Node->height));
	//printf("check test is %d\n", test);
	if (test <= 0) {
	  test = fscanf(ptr, "%c\n", &(Node->VH));
	  //printf("check test is %d\n", test);	
	  if (test > 0) {
	    Node->value = -1;
	    Node->right = pop(&stack);
	    Node->left = pop(&stack);
	    push(&stack, Node);
	  }
	  else{
	    free(Node);
	  }
	}  
	else {
	  Node->VH = 0;
	  push(&stack, Node);
	}
  }
  Node = pop(&stack);
  fclose(ptr);
  return Node;
}

void _Packing(TreeNode* root, double x, double y, double* outx, double* outy, int* value) {
  if (root -> left != NULL) {
	if (root->VH == 'V') {
	  _Packing(root->left, x, y, outx, outy, value);
	}
	else {
	  _Packing(root->left, x , y + root->height - root->left->height, outx, outy, value);
	}
  }
  if (root -> right != NULL) {
	if (root->VH == 'V') {
	  _Packing(root->right, x + root->width - root->right->width, y, outx, outy, value);
	}
	else {
	  _Packing(root->right, x, y, outx, outy, value);
	}
  }
  root->x = x;
  root->y = y;
  if (root->value > *value) { //root->value < '9' && 
	*value = root->value;
	*outx = root->x;
	*outy = root->y;
  }
  //printf("root is %c, x is %le, y is %le\n", root->value, root->x, root->y);
}

void _Perform_Packing(TreeNode* root, double* width, double* height, double* x, double* y, int* value) {
  if (root -> left != NULL) {
	_Perform_Packing(root->left, width, height, x, y, value);
  }
  if (root -> right != NULL) {
	_Perform_Packing(root->right, width, height, x, y, value);
  }
  if (root -> left != NULL) {
	if (root -> VH == 'V') {
	  root->width = root->left->width + root->right->width;
	  root->height = (root->left->height > root->right->height) ? root->left->height : root->right->height;
	}
	if (root -> VH == 'H') {////////////////////////////MARK
	  root->height = root->left->height + root->right->height;
	  root->width = (root->left->width > root->right->width) ? root->left->width : root->right->width;
	}
	//printf("wid = %le, hei = %le\n", root->width, root->height);
	//printf("l:%le, v:%c, r:%le\n", root->width, root->value, root->height);
  }
}

void Perform_Packing(TreeNode* root, double* width, double* height, double* x, double* y, int* value) {
  _Perform_Packing(root, width, height, x, y, value);
  _Packing(root, *x, *y, x, y, value);
}

void _Output_Tree(TreeNode* root, FILE* ptr) {
  if (root->left != NULL) {
	_Output_Tree(root->left, ptr);
  }
  if (root->right != NULL) {
	_Output_Tree(root->right, ptr);
  }
  if (root->left == NULL && root->right == NULL) {
    fprintf(ptr, "%d %le %le %le %le\n", (root->value), root->width, root->height, (root->x), (root->y));
	//printf("test is %d\n", test);
  }
}
void Save_Packing(char* Filename, TreeNode* root) {
  FILE* ptr = fopen(Filename, "w");
  _Output_Tree(root, ptr);
  fclose(ptr);
}
