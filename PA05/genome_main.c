#include <stdio.h>
#include <stdlib.h>
#include "genome.h"

void _main_print_array(int* array, int size_of_seq) {
	printf("array is: ");
	int i = 0;
	for(i = 0; i < size_of_seq; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	int size_of_seq = 0;
	int* Longest_array = Longest_conserved_gene_sequence(argv[1], &size_of_seq);
	printf("Length: %d\n", size_of_seq);
	_main_print_array(Longest_array, size_of_seq);
	free(Longest_array);

	return 0;
}



