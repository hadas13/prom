#include <stdio.h>
#include "Game.h"
#include "ILP.h"

Cell init_cell() {
	Cell init_cell;
	init_cell.val = 1;
	init_cell.is_fixed = 0;
	init_cell.is_err = 0;

	return init_cell;

}

int init_game_table(Board *board){
	int i;
	int n = board->n;

	board->game_table = (Cell **)malloc(n*sizeof(Cell *));
	if (board->game_table == NULL) {
		print_malloc_failed_err();
		free_board(board);
		exit(0);
	}

	for (i = 0; i < n; i++) {
	       board->game_table[i] = (Cell *)malloc(n*sizeof(Cell));	
	       if (board->game_table[i] == NULL) {
		       print_malloc_failed_err();
		       free_board(board);
		       exit(0);
	       }
	}

	return VALID;
}


Board *init_board(int n, int m_rows, int m_cols, int filled) {
	Board *board = NULL;

	board = (Board *)malloc(sizeof(Board));
	if (board == NULL) {
		print_malloc_failed_err();
		return NULL;
	}

	board->n = n;
	board->m_rows = m_rows;
	board->m_cols = m_cols;
	board->filled = filled;

	if (init_game_table(board) != VALID) {
		free_board(board);
		return NULL;
	}
	clean_vals_from_board(board);
	return board;
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
	game->curr_move = NULL;
	return game;
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

int play_mark_errors(Game *game, int is_mark) {
	if (game->game_mode != SOLVE) {
		print_err_invalid_command();
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
			is_err = !is_cell_valid(board, board->game_table, val, r, c);
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
		board->game_table[row][col].val = UNASSIGNED;
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

		if (board->game_table[row][col].val == UNASSIGNED){ /* an empty cell is filled */
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
	
	else if (board->game_table[Y-1][X-1].is_fixed) {
		print_err_cell_is_fixed();
		return NOT_VALID;
	}

	else if (board->game_table[Y-1][X-1].val != UNASSIGNED) {
		print_err_cell_assigned();
		return NOT_VALID;
	}

	clue = run_ILP(board, RUN_HINT, command.Y, command.X);
	
	if (clue == NOT_VALID) {
		print_err_unsolvable();
		return NOT_VALID;
	}

	print_hint(clue);
	return VALID;
}

/*void turn(struct Command command, Board *board, Game *game){*/
	/*int validated;*/

	/*if (command.valid == FALSE){ [> invalid command, get new command <]*/
		/*print_err_invalid_command()*/
		/*command = get_command();*/
		/*turn(command, board, game);*/
	/*}*/
	/*else{ [> valid command <]*/
		/*if ((board->filled == (board->n * board->n)) && (command.command != EXIT && command.command != RESTART)){*/
			/*[> board is filled, disable all command but exit and restart <]*/
			/*print_err_invalid_command();*/
			/*command = get_command();*/
			/*turn(command, board, game);*/
		/*}*/
		/*else { [> board is not filled, play according to command <]*/
			/*switch(command.command){*/
				/*case SET:*/
					/*play_set(command, board, game);*/
					/*break;*/
				/*case HINT:*/
					/*play_hint(command, board);*/
					/*break;*/
				/*case VALIDATE:*/
					/*validated = validate_game_table(board);*/
					/*if (validated == TRUE){*/
						/*printf("Validation passed: board is solvable\n");*/
					/*}*/
					/*else{*/
						/*printf("Validation failed: board is unsolvable\n");*/
					/*}*/
					/*break;*/
				/*case RESTART:*/
					/*board->filled = ((board->n * board->n) + 2); [> indication to restart the game <]*/
					/*break;*/
				/*case EXIT:*/
					/*board->filled = ((board->n * board->n) + 1); [> indication to exit the game <]*/
					/*break;*/
			/*}*/
		/*}*/
	/*}*/
/*}*/

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

	if (board->num_err != 0) {
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
					       	c + 1, r + 1, new_board->game_table[r][c].val);
				}
			}
		}
	}
	free_board(board);
	update_errors_on_board(new_board);
	return new_board;
}

/*void play(){*/
	/*int rows = 3; [> num of rows in a block <]*/
	/*int cols = 3; [> num of cols in a block <]*/
	/*int total_cells = (rows * cols); [> total numbers of cells in a line <]*/
	/*int fixed;*/
	/*Board *board;*/
	/*[>struct Command command;<]*/
	/*Game *game;*/

	/*fixed = get_fixed(); [> num of fixed cell <]*/
	/*game = init_game();*/
	/*board = init_board(total_cells, rows, cols, fixed); [> initial board <]*/
	/*print_board(board, game->mark_err);*/

	/*board = autofill(board, TRUE);*/

	/*while (!stop_game(board)){ [> keep playing <]*/
		/*if (board->filled == (board->n * board->n)){ [> the player won <]*/
			/*printf("Puzzle solved successfully\n");*/
			/*while(!stop_game(board)){ [> keep playing with only exit and restart commands <]*/
				/*[>command = get_command();<]*/
				/*[>turn(command, board, game);<]*/
			/*}*/
			/*break;*/
		/*}*/
		/*[>command = get_command();<]*/
		/*[>turn(command, board, game);<]*/
	/*}*/
	/*if (board->filled == ((board->n * board->n) + 2)){ [> restart command <]*/
		/*free_board(board);*/
		/*play();*/
	/*}*/
	/*else{ [> exit command <]*/
		/*printf("Exiting...\n");*/
		/*free_board(board);*/
	/*}*/
/*}*/


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
		if (run_ILP(board, RUN_VALIDATE, 0, 0) != VALID) {
			print_err_board_validate_failed();
			return NOT_VALID;
		}
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

int check_generate_valid(Game *game, Board *board, int x, int y) {
	int n = board->n;
	int unfilled_cells = n*n - board->filled;

	/* TODO - decide if it suppose to be here */
	if (game->game_mode != EDIT) {
		print_err_invalid_command();
		return NOT_VALID;
	}

	else if (board->filled != 0) {
		print_err_board_not_empty();
		return NOT_VALID;
	}

	else if (x > unfilled_cells || x < 0 || y > unfilled_cells || y < 0) { 
		print_err_value_not_int_range_E(unfilled_cells);
		return NOT_VALID;
	}

	return VALID;
}

int is_free_cell_position(int row, int col, Cell_Item *x_cells_arr, int num_cells_to_check){
	int i;
	for (i = 0; i < num_cells_to_check; i++) {
		if (x_cells_arr[i].row == row && x_cells_arr[i].col == col) {
			/* the given position in the array */
			return FALSE;
		}
	}
	/* the given position not in the array */
	return TRUE;
}


int get_random_legal_val(Board *board, int row, int col) {
	int *num_arr = NULL;
	int n = board->n;
	int next_num = NOT_POSSIBLE_VAL;

	num_arr = (int *)calloc((n + 1), sizeof(int));
	if (num_arr == NULL) {
		printf("Error: calloc has failed\n");
		free_board(board);
		exit(NOT_VALID);
	}

	get_possible_vals(board, board->game_table, row, col, num_arr);
	switch(num_arr[n]) { /* num_arr[n] is the counter of possible nums for the cell*/
		case (NON_OPTION):
			/* no possible number for this cell */
			free(num_arr);
			board->game_table[row][col].val = 0;
			return NOT_POSSIBLE_VAL;

		case (SINGLE_OPTION):
			next_num = get_first_option(num_arr, board->n, 0);
			break;

		default:
			/* more then one possible option - choose randomly a number */
			next_num = get_random_option(num_arr, n);
			break;
	}
	
	free(num_arr);
	return next_num;
}


int update_board_with_cells_arr(Cell_Item *arr, Board *board, int num_cells) {
	int i;
	int r, c, val;
	for (i = 0; i < num_cells; i++) {
		r = arr[i].row;
		c = arr[i].col;
		val = arr[i].val;
		/* update board */
		board->game_table[r][c].val = val;
		board->game_table[r][c].is_err = FALSE;
		board->game_table[r][c].is_fixed = FALSE; /* TODO check if it suppuse to be fixed or not */
	}

	update_errors_on_board(board);
	if (board->num_err != 0) {
		/* something went worng - have a error in the board that shouldn't be */
		return NOT_VALID;
	}
	return VALID;
}

int fill_x_empty_cells(Board *board, int x) {
	Cell_Item *x_cells_arr = NULL;
	int n = board->n;
	int counter = 0;
	int rand_row, rand_col, rand_val;

	x_cells_arr = (Cell_Item *)malloc(x*sizeof(Cell_Item));
	if (x_cells_arr == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	while (counter != x) {
		rand_row = rand() % n;
		rand_col = rand() % n;

		while (is_free_cell_position(rand_row, rand_col, x_cells_arr, counter) == FALSE) {
			rand_row = rand() % n;
			rand_col = rand() % n;
		}
		x_cells_arr[counter].row = rand_row;
		x_cells_arr[counter].col = rand_col;
		rand_val = get_random_legal_val(board, rand_row, rand_col);
		if (rand_val == NOT_POSSIBLE_VAL) {
			/* didn't find a legal value for the cell */
			return NOT_VALID;
		}

		x_cells_arr[counter].val = rand_val;
		counter += 1;
	}

	if (update_board_with_cells_arr(x_cells_arr, board, x) == NOT_VALID) {
		free(x_cells_arr);
		return NOT_VALID;
	}

	free(x_cells_arr);
	return VALID;
}

int remove_y_cells(Board *board, int y) {
	Cell_Item *y_cells_arr = NULL;
	int n = board->n;
	int counter = 0;
	int rand_row, rand_col;

	y_cells_arr = (Cell_Item *)malloc(y*sizeof(Cell_Item));
	if (y_cells_arr == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	while (counter != y) {
		rand_row = rand() % n;
		rand_col = rand() % n;
		while (is_free_cell_position(rand_row, rand_col, y_cells_arr, counter) == FALSE) {
			rand_row = rand() % n;
			rand_col = rand() % n;
		}
		y_cells_arr[counter].row = rand_row;
		y_cells_arr[counter].col = rand_col;
		/* empty cell value */
		y_cells_arr[counter].val = UNASSIGNED;
		counter += 1;
	}

	if (update_board_with_cells_arr(y_cells_arr, board, y) == NOT_VALID) {
		free(y_cells_arr);
		return NOT_VALID;
	}

	free(y_cells_arr);
	return VALID;
}


int play_generate(Game *game, Board *board, int x, int y){
	Cell_Item *x_cells_arr = NULL;
	int iter;
	int elem_to_remove = board->n * board->n - y;

	if (check_generate_valid(game, board, x, y) == NOT_VALID) {
		return NOT_VALID;
	}

	for (iter = 0; iter < NUM_ITER_GENERATE; iter++) {
		if ((fill_x_empty_cells(board, x) == VALID) && run_ILP(board, RUN_GENERATE, 0, 0)) {
			/* generated full board */
			remove_y_cells(board, elem_to_remove);
			print_board(board, game->mark_err);
			free(x_cells_arr);
			return VALID;
		}

		clean_vals_from_board(board);
	}

	print_err_generator_failed();
	clean_vals_from_board(board);
	free(x_cells_arr);
	return NOT_VALID;
}

int play_validate(Board *board) {
	if (board->num_err != 0) {
		print_errorneous_err();
		return NOT_VALID;
	}

	if (run_ILP(board, RUN_VALIDATE, 0, 0) == NOT_VALID) {
		print_validation_failed();
		return NOT_VALID;
	}
	else{
		print_validation_passed();
		return VALID;
	}
	return VALID;
}

int free_game(Game *game) {
	if (free_all_moves(game->curr_move) != VALID) {
		return NOT_VALID;
	}

	free(game);
	return VALID;
}
