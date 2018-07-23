#include <stdio.h>
#include "GUI.h"


int print_errorneous_err(){
	printf(CONTAINS_ERRORNEOUS_ERR);
	return VALID;
}

int print_cell_fixed_err(){
	printf(CELL_FIXED_ERR);
	return VALID;
}

int print_unsolvable_err(){
	printf(UNSOLVABLE_ERR);
	return VALID;
}

int print_num_of_solutions(int num){
	if (num < 0){
		return NOT_VALID;
	}

	printf("Number of solutions: %d\n", num);
	if (num == 1){
		/* single solution */
		printf("This is a good board!\n");
	}

	else if (num > 1){
		printf("The puzzle has more that 1 solution, try to edit it further\n");
	}

	return VALID;
}

int print_calloc_failed_err(){
	printf("Error: calloc has failed\n");
	return VALID;
}

