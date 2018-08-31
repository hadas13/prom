#include <stdlib.h>
#include "MainAux.h"


int string_to_int(char *string){
	char *end;
	int num = strtol(string, &end, 10);
	return (num);
}

int is_numeric_string(char *string){
	int i = 0;
	int numeric = 1;
	while (string[i] != '\0'){
		if (!isdigit(string[i])){
			numeric = 0;
			break;
		}
		i += 1;
	}
	return numeric;
}

int save_file_to(FILE *fd, Board *board, int mode) {
	int r, c;
	/* write the rows number and the columns number */
	fprintf(fd, "%d %d\n", board->m_rows, board->m_cols);

	for (r = 0; r < board->n; r++) {
		for (c = 0; c < board->n; c++) {
			fprintf(fd, "%d", board->game_table[r][c].val);
			if ((mode == EDIT) || (board->game_table[r][c].is_fixed)) {
				/* marked as fixed */
				fprintf(fd, ".");
			}

			if (c != (board->n-1)) {
				/* not end of column */
				fprintf(fd, " ");
			}
		}
		/* ended row */
		fprintf(fd, "\n");
	}
	return VALID;
}

int clean_vals_from_board(Board *board) {
	int c = 0;
	int r = 0;
	for(r = 0; r < board->n; r++) {
		for(c = 0; c < board->n; c++) {
			if (board->game_table[r][c].is_fixed == FALSE) {
				board->game_table[r][c].val = UNASSIGNED;
			}
		}
	}
	return VALID;
}

