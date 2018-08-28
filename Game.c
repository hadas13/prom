#include <stdio.h>
#include "Game.h"

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

int play_mark_errors(Game *game, int is_mark) {
	if (game->game_mode != SOLVE) {
		printf("Error: invalid command\n"); /* decided if to do it here */
		return NOT_VALID;
	}

	if (is_mark != TRUE && is_mark != FALSE) {
		print_err_is_mark_val();
		return NOT_VALID;
	}

	game->mark_err = is_mark;
	return VALID;
}
int is_row_col_param_valid(int N, int X, int Y) {
	if (X < 1 || X > N || Y < 1 || Y > N) {
		return NOT_VALID;
	}
	
	return VALID;
}

int is_set_param_valid(int N, int X, int Y, int Z) {
	if (!is_row_col_param_valid(N, X, Y)) {
		return NOT_VALID;
	}

	else if (Z < 0 || Z > N) {
		return NOT_VALID;
	}
	return VALID;
}

int update_errors_on_board(Board *board) {
	int n = board->n;
	int is_err = FALSE;
	int val = 0;
	int num_err = 0;
	int r, c = 0;

	for (r = 0; r < n; r++) {
		for (c = 0; c < n; c++) {
			val = board->game_table[r][c].val;
			is_err = is_cell_valid(board, board->game_table, val, r, c);
			board->game_table[r][c].is_err = is_err;
			if (is_err) {
				num_err += 1;
			}
		}
	}

	board->num_err = num_err;
	return VALID;
}

int filled_cell(Board *board, Game *game, int col, int row, int val) {
	int new_filled = board->filled;
	int is_valid; /* indication if number to fill is valid for given cell */
	int update_err = FALSE;

	if (board->game_table[row][col].is_err == TRUE) {
		/* the cell value was an error - needs to validate the board */
		update_err = TRUE;
	}

	/* filled cell, either enter a new num of clear */
	if (val == UNASSIGNED){ 
		if (board->game_table[row][col].val != UNASSIGNED) {
			/* player asked to clear a cell - number of filled cells decreased */
			board->filled = (new_filled - 1);
		}
		board->game_table[row][col].val = 0;
		board->game_table[row][col].is_err = FALSE;
		print_board(board, game->mark_err);
	}

       	else {
		is_valid = is_cell_valid(board, board->game_table, val, row, col);
		if (!is_valid) {
			/* inserting error value */
			update_err = TRUE;
			board->game_table[row][col].is_err = TRUE;
		}

		else {
			board->game_table[row][col].is_err = FALSE;
		}

		if (board->game_table[row][col].val == 0){ /* an empty cell is filled */
			board->filled = (new_filled + 1); /* number of filled cells increased by one */
		}

		board->game_table[row][col].val = val;
		board->game_table[row][col].is_fixed = FALSE;
	}
	
	if (update_err) {
		update_errors_on_board(board);
	}

	print_board(board, game->mark_err);
	return VALID;
}

void play_set(struct Command command, Board *board, Game *game){
	/* TODO - 1- needs to handle after inserting - ending game  */

	int X = command.X;
	int Y = command.Y;
	int Z = command.Z; /* number to fill */

	if (!is_set_param_valid(board->n, X, Y, Z)) {
		print_err_value_not_in_range();
		return;
	}
	
	else if (board->game_table[Y-1][X-1].is_fixed){ /* fixed cell */
		print_err_cell_is_fixed();
		return;
	}

	add_new_move(game->curr_move, Y, X, Z, board->game_table[Y-1][X-1].val);


	filled_cell(board, game, X-1 , Y-1, Z);
	return;

}

int play_hint(struct Command command, Board *board){
	/* TODO - check how to detrmine if X, Y are integers */
	int X = command.X;
	int Y = command.Y;
	int clue = 0;

	if (!is_row_col_param_valid(board->n, X, Y)) {
		print_err_row_col_not_in_range();
		return NOT_VALID;
	}

	else if (board->num_err > 0) {
		print_err_board_contains_error();
		return NOT_VALID;
	}
	
	else if (board->solution[Y-1][X-1].is_fixed) {
		print_err_cell_is_fixed();
		return NOT_VALID;
	}

	else if (board->solution[Y-1][X-1].val != UNASSIGNED) {
		print_err_cell_assigned();
		return NOT_VALID;
	}
	
	/* TODO - decide if we are using 2 board - one for solution, or we
	 * just running ILP here */

	clue = board->solution[Y-1][X-1].val;
	printf("Hint: set cell to %d\n", clue);
	return VALID;
}

void turn(struct Command command, Board *board, Game *game){
	int validated;

	if (command.valid == FALSE){ /* invalid command, get new command */
		printf("Error: invalid command\n");
		command = get_command();
		turn(command, board, game);
	}
	else{ /* valid command */
		if ((board->filled == (board->n * board->n)) && (command.command != EXIT && command.command != RESTART)){
			/* board is filled, disable all command but exit and restart */
			printf("Error: invalid command\n");
			command = get_command();
			turn(command, board, game);
		}
		else { /* board is not filled, play according to command */
			switch(command.command){
				case SET:
					play_set(command, board, game);
					break;
				case HINT:
					play_hint(command, board);
					break;
				case VALIDATE:
					validated = validate_game_table(board);
					if (validated == TRUE){
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

Game *init_game(){
	Game *game = NULL;

	game = (Game *)malloc(sizeof(Game));
	if (game == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	game->mark_err = TRUE; /* by default, the value is 1 */
	game->game_mode = INIT;
	return game;
}

int check_and_change_one_sol_cell(Cell **checking_board, int row, int col, Board *board){
	int changed = FALSE;
	int set_num = 0;
	int *num_opt = NULL;
	int n = board->n;

	num_opt = (int *)(calloc((n + 1), sizeof(int)));
	if (num_opt == NULL){
		print_calloc_failed_err();
		free_board(board);
		exit(0);
	}

	get_possible_vals(board, checking_board, row, col, num_opt);
	switch(num_opt[n]) { /* num_opt[n] is the counter of possible nums for the cell */
		case (NON_OPTION):
			changed = FALSE;
			break;

		case (SINGLE_OPTION):
			set_num = get_first_option(num_opt, n, 0);
			board->game_table[row][col].val = set_num;
			board->game_table[row][col].is_err = FALSE;
			board->game_table[row][col].is_fixed = FALSE;
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
	
Board *autofill(Board *board, int to_print) {
	int r, c;
	int changed = FALSE;
	Board *new_board;

	if (board == NULL){
		return NOT_VALID;
	}

	if (board->num_err > 0) {
		print_board_contains_error();
		return NOT_VALID;
	}

	new_board = init_board(board->n, board->m_rows, board->m_cols, board->filled);
	copy_board(board->game_table, new_board->game_table, board->n);

	for (r = 0; r < board->n; r++){
		for (c = 0; c < board->n; c++){
			if (board->game_table[r][c].val == UNASSIGNED) {
				changed = check_and_change_one_sol_cell(board->game_table, r, c, new_board);
				if (changed && to_print){
					printf("Cell <%d,%d> set to %d\n",
					       	r + 1, c + 1, new_board->game_table[r][c].val);
				}
			}
		}
	}
	free_board(board);
	return new_board;
}

void play(){
	int rows = 3; /* num of rows in a block */
	int cols = 3; /* num of cols in a block */
	int total_cells = (rows * cols); /* total numbers of cells in a line */
	int fixed;
	Board *board;
	struct Command command;
	Game *game;

	fixed = get_fixed(); /* num of fixed cell */
	game = init_game();
	board = init_board(total_cells, rows, cols, fixed); /* initial board */
	print_board(board, game->mark_err);

	board = autofill(board, TRUE);

	while (!stop_game(board)){ /* keep playing */
		if (board->filled == (board->n * board->n)){ /* the player won */
			printf("Puzzle solved successfully\n");
			while(!stop_game(board)){ /* keep playing with only exit and restart commands */
				command = get_command();
				turn(command, board, game);
			}
			break;
		}
		command = get_command();
		turn(command, board, game);
	}
	if (board->filled == ((board->n * board->n) + 2)){ /* restart command */
		free_board(board);
		play();
	}
	else{ /* exit command */
		printf("Exiting...\n");
		free_board(board);
	}
}


int play_num_solutions(Board *board) {
	int num_sol = 0;

	if (board->num_err != 0) {
		print_board_contains_error();
		return NOT_VALID;
	}

	num_sol = count_solutions(board);
	print_number_of_solutions(num_sol);
	
	if (num_sol == 1) {
		print_notify_one_solution();
	}

	else {
		print_notify_more_solutions();
	}

	return VALID;
}

int play_save(Board *board, Game *game, char *path) {
	FILE *fd;
	int is_save = FALSE;

	if (game->game_mode == EDIT) {
		if (board->num_err != 0) {
			print_board_contains_error();
			return NOT_VALID;
		}
		/* TODO - check it with omer */
		/*if (validate(board, RUN_VALIDATE) != VALID) {*/
                               /*print_err_board_validate_failed();*/
			/*return NOT_VALID;*/
		/*}*/
	}

	fd = fopen(path, "w");

	if (fd == NULL) {
		/* got an error */
		print_err_cant_created_or_modified();
		return NOT_VALID;
	}

	is_save = save_file_to(fd, board, game->game_mode);
	if (is_save) {
		print_save_file_to(path);
	}

	fclose(fd);
	return VALID;
}

/*int play_solve(char *path) {*/
	/*return VALID;*/

/*}*/
