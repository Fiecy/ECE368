#include <stdio.h>
#include <stdlib.h>



void* _return_stack_tail_node(void* _stack){
  if (_stack->right != NULL)
	return _return_stack_tail_node(_stack->right);

  return _stack;
}
void* _push(void* _stack, void* node){
  if (_stack == NULL || data == NULL)
	return _stack;

  void* ptr = NULL;
  ptr = _return_stack_tail_node(_stack);
  ptr->right = node;
  return _stack;
}

void* _pop(void* _stack){
  if (_stack == NULL)
	return NULL;

  void* ptr = NULL;
  ptr = _return_stack_tail_node(_stack);
  ptr->left->right = NULL;
  return ptr;

}


int main(int argc, char *argv[])
{
  
  struct node{
	int a;
	int b;
  };

  struct test{
	node* next;
	node* prev;
  };

  test* aaa;
  node* a;
  node* b;

  aaa = _push(aaa, a);


  return 0;
}
