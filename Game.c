#include <stdio.h>
#include "Game.h"

void print_frame(Board *board){
	/* prints "-" according to number of cells in block */
	int i = 0;
	int num = (((3 * board->m_cols) + 2) * board->m_rows) + 1;
	for (i = 1; i < num; i++){
		printf("-");
	}
	printf("-\n");
}

void print_board(Board *board){
	int n = board->n; /* num of cells in a row of board */
	int m_c = board->m_cols; /* num of cells in a column of block */
	int m_r = board->m_rows; /* num of cells in a row of block */
	int i = 0;
	int j = 0;
	int num;

	print_frame(board); /* print the top frame line */
	for (i = 0; i < n; i++){
		printf("|"); /* start a block */
		for (j = 0; j < n;j++){
			num = board->game_table[i][j];
			if (num < 0){ /* fixed cell */
				printf(" .%d", get_positive(num));
			}
			else if (num == 0){ /* empty cell */
				printf("   ");
			}
			else{ /* filled un fixed cell */
				printf("  %d", num);
			}
			if ((j+1) % m_c == 0){ /* frame of block */
				printf(" |");
			}
		}
		printf("\n");
		if ((i+1) % m_r == 0){ /* end of a block */
			print_frame(board);
		}
	}
}

int stop_game(Board *board){
	int stop;
	if (board->filled > (board->n * board->n)){ /* indicate exit of restart commands */
		stop = 1;
	}
	else{
		stop = 0;
	}
	return (stop);
}

void play_set(struct Command command, Board *board){
	int valid; /* indication if number to fill is valid for given cell */
	int X = command.X;
	int Y = command.Y;
	int Z = command.Z; /* number to fill */
	int new_filled = board->filled;
	if (board->game_table[Y-1][X-1] < 0){ /* fixed cell */
		printf("Error: cell is fixed\n");
	}
	else{ /* filled cell, either enter a new num of clear */
		if (Z == 0){ /* player asked to clear a cell */
			board->game_table[Y-1][X-1] = 0;
			board->filled = (new_filled - 1); /* number of filled cells decreased by one */
			print_board(board);
		}
		else{
			valid = is_cell_valid(board, board->game_table, Z, Y-1, X-1);
			if (valid == 1){ /* number is valid for given cell */
				if (board->game_table[Y-1][X-1] == 0){ /* an empty cell is filled */
					board->game_table[Y-1][X-1] = Z;
					board->filled = (new_filled + 1); /* number of filled cells increased by one */
					print_board(board);
				}
				else { /* a filled cell's number is changed */
					board->game_table[Y-1][X-1] = Z;
					print_board(board);
				}
			}
			else{
				printf("Error: value is invalid\n");
			}
		}

	}
}

void play_hint(struct Command command, Board *board){
	int X = command.X;
	int Y = command.Y;
	int clue = board->solution[Y-1][X-1];
	clue = get_positive(clue); /* absolute value in case number is fixed (represented as negative) */
	printf("Hint: set cell to %d\n", clue);
}

void turn(struct Command command, Board *board){
	int validated;

	if (command.valid == 0){ /* invalid command, get new command */
		printf("Error: invalid command\n");
		command = get_command();
		turn(command, board);
	}
	else{ /* valid command */
		if ((board->filled == (board->n * board->n)) && (command.command != EXIT && command.command != RESTART)){
			/* board is filled, disable all command but exit and restart */
			printf("Error: invalid command\n");
			command = get_command();
			turn(command, board);
		}
		else { /* board is not filled, play according to command */
			switch(command.command){
				case SET:
					play_set(command, board);
					break;
				case HINT:
					play_hint(command, board);
					break;
				case VALIDATE:
					validated = validate_game_table(board);
					if (validated == 1){
						printf("Validation passed: board is solvable\n");
					}
					else{
						printf("Validation failed: board is unsolvable\n");
					}
					break;
				case RESTART:
					board->filled = ((board->n * board->n) + 2); /* indication to restart the game */
					break;
				case EXIT:
					board->filled = ((board->n * board->n) + 1); /* indication to exit the game */
					break;
			}
		}
	}
}

int get_fixed(){
	int fixed = -1;
	printf("Please enter the number of cells to fill [0-80]:\n");
	if (scanf("%d", &fixed) < 0){
		if (fixed == EOF){ /* EOF */
			printf("Exiting...\n");
		}
		else{ /* scanf failed */
			printf("Error: scanf has failed\n");
		}
		exit(0);
	}
	while (fixed < 0 || fixed > 80){ /* fixed cell is invalid number */
		printf("Error: Invalid number of cells to fill (should be between 0 and 80)\n");
		printf("Please enter the number of cells to fill [0-80]:\n");
		if (scanf("%d", &fixed) < 0){
				if (fixed == EOF){ /* EOF */
					printf("Exiting...\n");
				}
				else{ /* scanf failed */
					printf("Error: scanf has failed\n");
				}
				exit(0);
			}
		}
	return (fixed);
}

void play(){
	int rows = 3; /* num of rows in a block */
	int cols = 3; /* num of cols in a block */
	int total_cells = (rows * cols); /* total numbers of cells in a line */
	int fixed;
	Board *board;
	struct Command command;

	fixed = get_fixed(); /* num of fixed cell */
	board = init_game(total_cells, rows, cols, fixed); /* initial board */
	print_board(board);

	while (!stop_game(board)){ /* keep playing */
		if (board->filled == (board->n * board->n)){ /* the player won */
			printf("Puzzle solved successfully\n");
			while(!stop_game(board)){ /* keep playing with only exit and restart commands */
				command = get_command();
				turn(command, board);
			}
			break;
		}
		command = get_command();
		turn(command, board);
	}
	if (board->filled == ((board->n * board->n) + 2)){ /* restart command */
		clean_up(board);
		play();
	}
	else{ /* exit command */
		printf("Exiting...\n");
		clean_up(board);
	}
}

int check_and_change_one_sol_cell(int **checking_board, int row, int col, Board *board){
	int changed = FALSE;
	int set_num = 0;
	int *num_opt = NULL;
	int n = board->n;

	num_opt = (int *)(calloc((n + 1), sizeof(int)));
	if (num_opt == NULL){
		print_calloc_failed_err();
		clean_up(board);
		exit(0);
	}

	get_possible_vals(board, checking_board, row, col, num_opt);
	switch(num_opt[n]) { /* num_opt[n] is the counter of possible nums for the cell */
		case (NON_OPTION):
			changed = FALSE;
			break;

		case (SINGLE_OPTION):
			set_num = get_first_option(num_opt, n, 0);
			board->game_table[row][col] = set_num;
			changed = TRUE;
			break;

		default:
			/* more than one solution to this cell */
			changed = FALSE;
			break;
	}
	
	free(num_opt);
	return changed;
}
	

int autofill(Board *board,int **checking_grid){
	int r, c;
	int changed = FALSE;
	Board *new_board;

	if (checking_grid == NULL || board == NULL){
		return NOT_VALID;
	}

	new_board = init_game(board->n, board->m_rows, board->m_cols, board->filled);
	copy_board(board->game_table, new_board->game_table, board->n);

	for (r = 0; r < board->n; r++){
		for (c = 0; c < board->n; c++){
			/* TODO: add errorneous values here:
			 * if board[r][c] == ERRORNEOUS: 
			 *	print_errorneous_err(); */
			changed = check_and_change_one_sol_cell(checking_grid, r, c, new_board);
			if (changed){
				printf("Cell <%d,%d> set to %d\n",
					       	r, c, new_board->game_table[r][c]);
			}
		}
	}
	return VALID;
}

