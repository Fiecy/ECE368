#include "river.h"

void _free_matrix(matrix* array) {
	size_t i = 0;
	for (i = 0; i < (array->n-1); i++) {
		free(array->mat[i]);
	}
	free(array->mat);
	free(array);
}

void _print_matrix(matrix* array) {
	printf("2D matrix:\n");
	size_t i = 0; size_t j = 0;
	for (i = 0; i < array->n-1; i++) {
		for (j = 0; j < array->m; j++) {
			printf(" %d",(array->mat)[i][j]);
		}
		printf("\n");
	}
	printf("end matrix\n");
}

matrix* _create_matrix(int n, int m) {
	matrix* test_matrix = malloc(sizeof(matrix));
	test_matrix->n = n+1;
	test_matrix->m = m;
	test_matrix->mat = malloc(n*sizeof(size_t));
	int i = 0;
	for (i = 0; i < n; i++) {
		test_matrix->mat[i] = malloc(m*sizeof(size_t));
	}
	return test_matrix;
}

matrix* Load_From_File(char* filename) {
	FILE* ptr = fopen(filename, "r");
	int test = 0;
	int m = 0;
	int n = 0;

	test = fscanf(ptr, "%d %d\n", &n, &m);

	matrix* array = _create_matrix(n-1, m);
	//printf("n = %d, m = %d\n", array->n, array->m);

	int i;
	int j;
	char temp = 0;

	for (i = 0; i < (n-1); i++) {
		for (j = 0; j < m-1; j++) {
			test = fscanf(ptr, "%c", &temp);
			array->mat[i][j] = temp - 48;
		}
		if (i < n-2) {
			test = fscanf(ptr, "%c\n", &temp);
		}
		else{
			test = fscanf(ptr, "%c", &temp);
		}
		array->mat[i][j] = temp - 48;
	}
	
	fclose(ptr);
	return array;
}
void _build_tree(TreeNode** Node, matrix* array) {
	int i,j;
	for (i = 0; i < (array->n-1); i++) {  // assign up and down
		for (j = 0; j < array->m; j++) {
			if (array->mat[i][j] == 1) {
				Node[i][j].down = 2;
				Node[i+1][j].up = 2;
			}
			else {
				Node[i][j].down = 0;
				Node[i+1][j].up = 0;
			}
		}
		Node[i][j].up = -2;
		Node[i][j].down = -2;
	}
	for (i = 0; i < (array->n); i++) {  // assign left right value and i, j for all of the elements
		for (j = 0; j < array->m+1; j++) {
			Node[i][j].left = 0;
			Node[i][j].right = 0;
			Node[i][j].value = 2 * (array->m);
			Node[i][j].i = i;
			Node[i][j].j = j;
			Node[i][j].next = NULL;
			Node[i][j].prev = NULL;
		}
	}
	for (i = 0; i < (array->n); i++) {  // adjust first and last col
		Node[i][0].left = -2;
		Node[i][0].value = 0;
		Node[i][array->m].right = -2;
		Node[i][array->m].left = -2;
		Node[i][array->m].up = -2;
		Node[i][array->m].down = -2;
	}
	for (j = 0; j < (array->m+1); j++) {  // adjust top and bottom row
		Node[0][j].up = -2;
		Node[array->n-1][j].down = -2;
	}
}

void _free_tree(TreeNode** Node, int n) {
	int i;
	for (i = 0; i < n; i++) {
		free(Node[i]);
	}
	free(Node);
}

void _print_tree(TreeNode** Node, int n, int m) {
	int i,j;
	printf("Print Tree Start\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			printf("%d ", Node[i][j].value); // test down first
		}
		printf("\n");
	}
}

void _push_2(TreeNode* stack, TreeNode* Node) {
	if (stack->value >= Node->value) {
		Node->prev = stack->prev;
		stack->prev->next = Node;
		Node->next = stack;
		stack->prev = Node;
		return;
	}
	else if (stack->next == NULL) {
		Node->prev = stack;
		Node->next = NULL;
		stack->next = Node;
		return;
	}
	else {
		_push_2(stack->next, Node);
		return;
	}
}
void _push(TreeNode** stack, TreeNode* Node) {
	if (*stack == NULL) {
		Node->prev = NULL;
		Node->next = NULL;
		*stack = Node;
		return;
	}
	if ((*stack)->value >= Node->value) {
		Node->prev = (*stack)->prev;
		Node->next = *stack;
		(*stack)->prev = Node;
		*stack = Node;
		return;
	}
	if ((*stack)->next == NULL) {
		Node->prev = *stack;
		Node->next = NULL;
		(*stack)->next = Node;
		return;
	}
	else {
		_push_2((*stack)->next, Node);
		return;
	}
}

void _pop_op2(TreeNode* stack, TreeNode* Node) {
	if (stack == NULL) {
		return;
	}
	if (stack->i != Node->i || stack->j != Node->j) { // not current Node
		_pop_op2(stack->next, Node);
		return;
	}
	else { //Node found
		Node->prev->next = Node->next;
		if (stack->next != NULL) {
			Node->next->prev = Node->prev;
		}
		else {
			Node->prev->next = NULL;
		}
		Node->prev = NULL;
		Node->next = NULL;
	}
}

void _pop_op(TreeNode** stack, TreeNode* Node) {
	if ((*stack)->i != Node->i || (*stack)->j != Node->j) { // not head
		_pop_op2((*stack)->next, Node);
		return;
	}
	TreeNode* temp = *stack; // head
	if ((*stack)->next != NULL) {	
		*stack = (*stack)->next;
		(*stack)->prev = NULL;
	}
	else{
		*stack = NULL;
	}
	temp->prev = NULL;
	temp->next = NULL;
}

TreeNode* _pop(TreeNode** stack) {
	if (*stack == NULL) { // empty
		printf("stack is empty!!\n");
		return NULL;
	}
	if ((*stack)->next == NULL) { // only one
		TreeNode* temp = *stack;
		*stack = NULL;
		temp->prev = NULL;
		return temp;
	}
	TreeNode* temp = *stack;
	(*stack)->next->prev = NULL;
	*stack = (*stack)->next;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}

void _print_stack(TreeNode* stack) {
	if (stack == NULL) {
		printf("empty stack\n");
		return;
	}
	printf("stack[%d][%d] contains %d\n", stack->i, stack->j, stack->value);
	if (stack->next != NULL) {
		_print_stack(stack->next);
	}
}

void _go_up(TreeNode** stack, TreeNode** Tree, TreeNode* Node) {
	int length = 2;
	TreeNode* op = &(Tree[Node->i-1][Node->j]);
	if (Node->up == 2) {
		length = 0;
	}
	else if (Node->left > 0 || Node->right > 0) {
		length = 1;
	}

	if (op->value > (Node->value + length)) { // update value and prev [set to 1] delete old prev [set to -1]
		op->value = (Node->value + length);
		op->down = 1;
		if (op->right == 1) op->right = -1;
		if (op->up == 1) op->up = -1;
		if (op->left == 1) op->left = -1;
	}
	else if (op->value == (Node->value + length)) { // update prev [set to 1]
		op->down = 1;
	}
	else { // useless path, delete it [set to -1]
		op->down = -1;
	}


	if (op->prev != NULL || op->next != NULL) {
	    _pop_op(stack, op);
	}

	_push(stack, op);
	//_print_stack(*stack);
	return;
}

void _go_down(TreeNode** stack, TreeNode** Tree, TreeNode* Node) {
	int length = 2;
	TreeNode* op = &(Tree[Node->i+1][Node->j]);
	if (Node->down == 2) {
		length = 0;
	}
	else if (Node->left > 0 || Node->right > 0) {
		length = 1;
	}

	if (op->value > (Node->value + length)) { // update value and prev [set to 1] delete old prev [set to -1]
		op->value = (Node->value + length);
		op->up = 1;
		if (op->right == 1) op->right = -1;
		if (op->down == 1) op->down = -1;
		if (op->left == 1) op->left = -1;
	}
	else if (op->value == (Node->value + length)) { // update prev [set to 1]
		op->up = 1;
	}
	else { // useless path, delete it [set to -1]
		op->up = -1;
	}


	if (op->prev != NULL || op->next != NULL) {
	    _pop_op(stack, op);
	}

	_push(stack, op);
	//_print_stack(*stack);
	return;
}

void _go_left(TreeNode** stack, TreeNode** Tree, TreeNode* Node) {
	int length = 2;
	TreeNode* op = &(Tree[Node->i][Node->j-1]);

	if (Node->up > 0 || Node->down > 0) {
		length = 1;
	}

	if (op->value > (Node->value + length)) { // update value and prev [set to 1] delete old prev [set to -1]
		op->value = (Node->value + length);
		op->right = 1;
		if (op->down == 1) op->down = -1;
		if (op->up == 1) op->up = -1;
		if (op->left == 1) op->left = -1;
	}
	else if (op->value == (Node->value + length)) { // update prev [set to 1]
		op->right = 1;
	}
	else { // useless path, delete it [set to -1]
		op->right = -1;
	}

	if (op->prev != NULL || op->next != NULL) {
	    _pop_op(stack, op);
	}

	_push(stack, op);
	return;
}

void _go_right(TreeNode** stack, TreeNode** Tree, TreeNode* Node) {
	int length = 2;
	TreeNode* op = &(Tree[Node->i][Node->j+1]);

	if (Node->up > 0 || Node->down > 0) {
		length = 1;
	}

	if (op->value > (Node->value + length)) { // update value and prev [set to 1] delete old prev [set to -1]
		op->value = (Node->value + length);
		op->left = 1;
		if (op->down == 1) op->down = -1;
		if (op->up == 1) op->up = -1;
		if (op->right == 1) op->right = -1;
	}
	else if (op->value == (Node->value + length)) { // update prev [set to 1]
		op->left = 1;
	}
	else { // useless path, delete it [set to -1]
		op->left = -1;
	}

	if (op->prev != NULL || op->next != NULL) {
	    _pop_op(stack, op);
	}

	_push(stack, op);
	return;
}

void _calculate_Node(TreeNode** stack, TreeNode** Tree, TreeNode* Node) {//////////////////////////////////
	//printf("Node i = %d, j = %d, L %d, R %d, U %d, D %d\n", Node->i, Node->j, Node->left, Node->right, Node->up, Node->down);
	if (Node->left == -2) { // begining Node
		_go_right(stack, Tree, Node);
		Node->up = -1;
		Node->down = -1;
		Node->right = -1;
		return;
	}

	if (Node->right == 0 || Node->right == 2) {
		_go_right(stack, Tree, Node);
		Node->right = -1;
	}	

	if (Node->left == 0 || Node->left == 2) {
		_go_left(stack, Tree, Node);
		Node->left = -1;
	}

	if (Node->up == 0 || Node->up == 2) {
		//printf("Up Node i = %d, j = %d, L %d, R %d, U %d, D %d\n", Node->i, Node->j, Node->left, Node->right, Node->up, Node->down);
		_go_up(stack, Tree, Node);
		Node->up = -1;
	}

	if (Node->down == 0 || Node->down == 2) {
		//printf("Down Node i = %d, j = %d, L %d, R %d, U %d, D %d\n", Node->i, Node->j, Node->left, Node->right, Node->up, Node->down);
		_go_down(stack, Tree, Node);
		Node->down = -1;
	}

	return;
}


void _Dijk(TreeNode** stack, TreeNode** Tree) {
	while (*stack != NULL) {
		TreeNode* Node = _pop(stack); // get the Node with least value
		_calculate_Node(stack, Tree, Node);
		//printf("start to print stack\n");
		//_print_stack(*stack);
	}
}

void _find_min(TreeNode** Tree, int n, int m) {
	int i = 0;
	int min = 2 * m;
	for (i = 0; i < n; i++) {
		if (min > Tree[i][m].value){
			min = Tree[i][m].value;
		}
		//printf("Tree[%d][%d] is %d\n", i, m, Tree[i][m].value);
	}
	printf("%d\n", min);
}
		
void Find_Shortest(matrix* array) {
	int i;
	TreeNode* stack = NULL;
	TreeNode** Tree = malloc(sizeof(TreeNode*) * array->n);
	for (i = 0; i < array->n; i++) {
		Tree[i] = malloc(sizeof(TreeNode) * (array->m+1));
	}
	_build_tree(Tree, array);
	//_print_tree(Tree, array->n, array->m+1);


	for (i = 0; i < array->n; i++) {
		_push(&stack, &(Tree[i][0]));
	}

	_Dijk(&stack, Tree);


	//_print_tree(Tree, array->n, array->m+1);
	//_print_stack(stack);
	_find_min(Tree, array->n, array->m);
	_free_tree(Tree, array->n);

}
