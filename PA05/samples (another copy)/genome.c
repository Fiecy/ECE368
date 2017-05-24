#include <stdio.h>
#include <stdlib.h>
#include "genome.h"
typedef struct {
	size_t n;
	size_t **mat;
}matrix;

typedef struct _find_array{
	struct _find_array* prev;
	struct _find_array* next;
	size_t value;
	size_t num;
} TreeNode;

void _free_matrix(matrix* array) {
	size_t i = 0;
	for (i = 0; i < array->n; i++) {
		free(array->mat[i]);
	}
	free(array->mat);
	free(array);
}

void _print_array(int* array, int size_of_seq) {
	printf("inside array is: ");
	size_t i = 0;
	for(i = 0; i < size_of_seq; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

matrix* _create_matrix(size_t n) {
	matrix* test_matrix = malloc(sizeof(matrix));
	test_matrix->n = n;
	test_matrix->mat = malloc(test_matrix->n*sizeof(size_t));
	size_t i = 0;
	for (i = 0; i < test_matrix->n; i++) {
		test_matrix->mat[i] = malloc(test_matrix->n*sizeof(size_t));
	}
	return test_matrix;
}

matrix* _create_zero_matrix(size_t n) {
	matrix* test_matrix = malloc(sizeof(matrix));
	test_matrix->n = n;
	test_matrix->mat = malloc(test_matrix->n*sizeof(size_t));
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < test_matrix->n; i++) {
		test_matrix->mat[i] = malloc(test_matrix->n*sizeof(size_t));
	}
	for (i = 0; i < test_matrix->n; i++) {
		for (j = 0; j < test_matrix->n; j++) {
			test_matrix->mat[i][j] = 0;
		}
	}
	return test_matrix;
}

void _print_matrix(matrix* array) {
	printf("2D matrix:\n");
	size_t i = 0; size_t j = 0;
	for (i = 0; i < array->n; i++) {
		for (j = 0; j < array->n; j++) {
			printf(" %ld",(array->mat)[i][j]);
		}
		printf("\n");
	}
	printf("end matrix\n");
}

void _print_tree(TreeNode* root) {
	if (root != NULL) {
		printf("%ld %ld\n", root->num, root->value);
		_print_tree(root->next);
	}
}

void _add_all_sequence(matrix* matrix1, int* array) {
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < matrix1->n; i++) {
		for (j = 0; j < i; j++) {
			matrix1->mat[array[i]-1][array[j]-1] = 0;
		}
	}
}

void _create_first_sequence(matrix* matrix1, int* array) {
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < matrix1->n; i++) {
		for (j = i; j < matrix1->n; j++) {
			matrix1->mat[array[i]-1][array[j]-1] = 1;
		}
		matrix1->mat[array[i]-1][array[i]-1] = 0;
	}
}

matrix* Load_From_File(char* filename, size_t* m, int** check_array) {//, int** check_array
	FILE* ptr = fopen(filename, "r");
	int test = 1;
	size_t i = 0;
	size_t n = 0;

	test = fread(&n, sizeof(int), 1, ptr);
	test = fread(m, sizeof(int), 1, ptr);

	matrix* matrix1 = _create_zero_matrix(n);
	matrix1->n = n;

	int* temp_array = malloc(sizeof(int) * n);
	test = fread(temp_array, sizeof(int)*n, 1, ptr);
	_create_first_sequence(matrix1, temp_array);

	for (i = 1; i < *m; i++) {
		test = fread(temp_array, sizeof(int)*n, 1, ptr);
		_add_all_sequence(matrix1, temp_array);	
	}		

	TreeNode* test_big = malloc(sizeof(TreeNode) * n);
	free(test_big);

	*check_array = temp_array;
	fclose(ptr);
	return matrix1;
}

void _find_len(matrix* matrix1, TreeNode* root, int* array) {
	size_t i,j;
	size_t len = 0;
	TreeNode* temp = NULL;
	for (i = 0; i < matrix1->n; i++) {
		len = 0;
		temp = NULL;
		for (j = 0; j < i; j++) { // number index before i
			if (matrix1->mat[array[j]-1][array[i]-1] == 1) { // go through the col of i [not row]
				if (root[array[j]-1].value > len) { // if previous number has a larger length, update.
					len = root[array[j]-1].value;
					temp = &(root[array[j]-1]);
				}
			}
		}
		root[array[i]-1].num = array[i];
		root[array[i]-1].value = len+1;
		root[array[i]-1].prev = temp; // if temp == NULL, root[].prev = NULL. root[] is the head of the list

		if (i < matrix1->n-1) {
			root[i].next = &(root[i+1]);
		}
		else{
			root[i].next = NULL;
		}
	}
}

int _return_max_index(TreeNode* root) {
	size_t cnt = 0;
	size_t i = 0;
	size_t return_i = 0;
	 while (root != NULL){
		if (root->value > cnt) {
			cnt = root->value;
			return_i = i;
		}
		root = root->next;
		i++;
	}
	return return_i;
}

void _print_length_list(TreeNode* root, int i) {
	if (root != NULL) {
		_print_length_list(root->prev, i+1);
		printf("index is %d, depth is %ld, num is %ld\n", i, root->value, root->num);
	}
}

int* _convert_list_to_array(TreeNode* root) {
	int* return_array = malloc(sizeof(int) * root->value);
	int i = root->value-1;

	while(root != NULL) {
		return_array[i] = root->num;
		root = root->prev;
		i--;
	}

	return return_array;
}

int* Longest_conserved_gene_sequence(char *filename, int *size_of_seq) {

	size_t m = 0;
	int* check_array = NULL;
//  build the n*n adjacency matrix  O(m*(n^2))
	matrix* standard_matrix = Load_From_File(filename, &m, &check_array);//, &check_array
	//printf("Matrix built successfully!\n\n");

//  create n TreeNode to save each number.
	TreeNode* root = malloc(sizeof(TreeNode) * standard_matrix->n);
//  build a linked list that contain the depth of each node  O(n^2)  ==> actually O((n^2)/2)......
	_find_len(standard_matrix, root, check_array);

//  free n*n matrix since it's not necessary in the following steps.
	free(check_array);
	_free_matrix(standard_matrix);

//  return the deepest number  O(n)
	size_t index = _return_max_index(root);
	*size_of_seq = root[index].value;

//  print the largest subnode.  O(n)  ##### use for test.
	//_print_length_list(&(root[index]), 0);
	//printf("\nsize is %d\n", *size_of_seq);

//  convert linked list to array.  O(n)
	int* return_array = _convert_list_to_array(&(root[index]));

	free(root);

	return return_array;
}
