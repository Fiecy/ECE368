#include "river.h"

int main(int argc, char * argv[]){
	matrix* array = NULL;	
	array = Load_From_File(argv[1]);
	//_print_matrix(array);

	Find_Shortest(array);
	_free_matrix(array);
	return 0;
}