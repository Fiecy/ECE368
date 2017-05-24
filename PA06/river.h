#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int n;
	int m;
	int** mat;
}matrix;

typedef struct _Node{
	int up;
	int down;
	int left;
	int right;
	int value;
	int i;
	int j;
	struct _Node* next;
	struct _Node* prev;
}TreeNode;

matrix* Load_From_File(char* filename);
void _print_matrix(matrix* array);
void _free_matrix(matrix* array);
void Find_Shortest(matrix* array);

