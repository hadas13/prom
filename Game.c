#include "Game.h"
/* 
 * this module is the implement of the game functions and logic. its implement all the gmae
 * functions
 */

/********** functions **********/
/*
 * function that initiates a cell struct
 */
Cell init_cell() {
	Cell init_cell;
	init_cell.val = 1;
	init_cell.is_fixed = 0;
	init_cell.is_err = 0;

	return init_cell;
}

/*
 * function that receives a board struct and initiates its game_table according to the board's parameters
 */
int init_game_table(Board *board){
 	int i = 0, r = 0, c = 0;
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
	for (r = 0; r < n; r++) {
			for (c = 0; c < n; c++) {
				board->game_table[r][c].val = UNASSIGNED;
				board->game_table[r][c].is_err = FALSE;
				board->game_table[r][c].is_fixed = FALSE;
			}
	}
	return VALID;
}

/*
 * function that receives board parameters and initiates a board struct accordingly
 */
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
	board->num_err = 0;
	board->game_table = NULL;

	if (init_game_table(board) != VALID) {
		free_board(board);
		return NULL;
	}
	clean_vals_from_board(board);
	return board;
}

/*
 * function that receives a board struct and count the number of filled cells in the board
 */
int count_num_of_filled_cells(Board *board) {
	int cnt = 0, row = 0, col = 0;
	for (row = 0; row < board->n; row++) {
		for (col = 0; col < board->n; col++) {
			if (board->game_table[row][col].val != UNASSIGNED) {
				cnt += 1;
			}
		}
	}
	return cnt;
}

/*
 * function that plays the mark errors command
 */
int play_mark_errors(Game *game, int is_mark) {
	if (game->game_mode != SOLVE_MODE) {
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

/*
 * function that receives indexes of row and col and a size of a board and checks if the row and col are legal for the board
 */
int is_row_col_param_valid(int N, int X, int Y) {
	if (X < 1 || X > N || Y < 1 || Y > N) {
		return NOT_VALID;
	}
	
	return VALID;
}

/*
 * function that receives the numeric parameters of an input set command and checks
 * if they are legal for the current game
 */
int is_set_param_valid(int N, int X, int Y, int Z) {
	if (!is_row_col_param_valid(N, X, Y)) {
		return NOT_VALID;
	}

	else if (Z < 0 || Z > N) {
		return NOT_VALID;
	}
	return VALID;
}

/*
 * function that goes over a board and marks each cell as erroneous or not according to sudoku rules
 */
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

/* 
 * insert val to cell row,col to board game
 */
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

	print_board(board, game->mark_err, game);
	return VALID;
}

/*
 * function that plays the set command
 */
int play_set(struct Command command, Board *board, Game *game){
	int row = 0, col = 0, value = 0;
	MoveInfo new_move;

	col = command.X - 1;
	row = command.Y - 1;
	value = command.Z; /* number to fill */
	init_empty_game_move_info(&new_move);

 	if (!is_set_param_valid(board->n, col + 1, row + 1, value)) {
 		print_err_value_not_in_range(board->n);
		return NOT_VALID;
	}
	
 	else if (board->game_table[row][col].is_fixed){ /* fixed cell */
 		print_err_cell_is_fixed();
		return NOT_VALID;
	}

	if (create_new_move(board, &new_move, row, col, value) != VALID) {
		printf("didn't created new move to list of moves\n");
		return NOT_VALID;
	}

	if (append_new_move_to_moves_list(game->curr_move, new_move) != VALID) {
		printf("Error: couldn't append new move for set: append_new_move_to_moves_list\n");
		return NOT_VALID;
	}

 	game->curr_move = game->curr_move->next;

	filled_cell(board, game, col, row, value);
	return VALID;
}

/*
 * function that plays the hint command
 */
int play_hint(struct Command command, Board *board){
	int X = command.X;
	int Y = command.Y;
	int clue = 0;
 	int row, col;

	if (!is_row_col_param_valid(board->n, X, Y)) {
		print_err_row_col_not_in_range(board->n);
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

	row = Y; /* no -1 needed to run with ILP */
	col = X; /* no -1 needed to run with ILP */
	clue = run_ILP(board, RUN_HINT, row, col);
	
	if (clue == NOT_VALID) {
		print_err_unsolvable();
		return NOT_VALID;
	}

	print_hint(clue);
	return VALID;
}

/* this functions checks if row and column can be filled with only one option,
 * if it is - it fills it with that value, and return as VALID, else - not valid.
 */
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
			changed = set_num;
			break;

		default:
			/* more than one solution to this cell */
			changed = FALSE;
			break;
	}
	
	free(num_opt);
	return changed;
}

/*
 * function that checks if the board is eligible for an autofill command
 */
int check_autofill_param(Board *board) {

	if (board == NULL){
		return NOT_VALID;
	}

	else if (update_errors_on_board(board) != VALID) {
		printf("Error: failed updating errors on board - autofill\n");
		return NOT_VALID;
	}
	else if (board->num_err != 0) {
		print_board_contains_error();
		return NOT_VALID;
	}

	return VALID;
}

/*
 * this function is the logic of autofill
 */
Board *autofill(Board *board, Game *game, int to_print, MoveList **chain) {
	int r, c;
	int is_something_changed = FALSE;
	int changed = FALSE;
	Board *new_board;
	MoveList *autofill_changes;
	MoveInfo cell_change, autofill_set;

	new_board = init_board(board->n, board->m_rows, board->m_cols, board->filled);
	copy_board(board->game_table, new_board->game_table, board->n);

	init_empty_game_move_info(&autofill_set);
	init_empty_game_move_info(&cell_change);

	for (r = 0; r < board->n; r++){
		for (c = 0; c < board->n; c++){
			if (board->game_table[r][c].val == UNASSIGNED) {
				changed = check_and_change_one_sol_cell(board->game_table, r, c, new_board);
				if (changed != UNASSIGNED) {
					if (is_something_changed == FALSE) {
						/* first time that something changed in the board */
						is_something_changed = TRUE;
						if (create_empty_chain(&autofill_changes) != VALID) {
							printf("Error: couldn't init move list for autofill\n");
							return NULL;
						}
						*chain = autofill_changes;
						autofill_set.subchain = autofill_changes;
					}
					if (create_new_move(board, &cell_change,
							       	r, c, changed) != VALID) {
						printf("Error: could create new move in	autofill, after found change in board\n");
						return NULL;
					}
					append_new_move_to_moves_list(autofill_changes, cell_change);
					autofill_changes = autofill_changes->next;
					if (to_print){
						printf("Cell <%d,%d> set to %d\n",
						c + 1, r + 1, changed);
					}
				}
			}
		}
	}

	if (is_something_changed) {
		/* should changed the move list */
		if (append_new_move_to_moves_list(game->curr_move, autofill_set) != VALID) {
			return NOT_VALID;
		}
		game->curr_move = game->curr_move->next;
		board->filled = count_num_of_filled_cells(board);
		update_errors_on_board(new_board);
	}

	free_board(board);
	return new_board;
}

/*
 * function that plays the autofill command
 */
int play_autofill(Board **board, Game *game, int to_print) {
	MoveList *chain;

	if (check_autofill_param(*board) != VALID) {
		return NOT_VALID;
	}
	*board = autofill(*board, game, to_print, &chain);
	if (board == NULL) {
		printf("Error: couldn't get new board of autofill\n");
		return NOT_VALID;
	}
	print_board(*board, game->mark_err, game);
	return VALID;
}

/*
 * function that plays the num solutions command
 */
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

	else if (num_sol > 1) {
		print_notify_more_solutions();
	}

	return VALID;
}

/*
 * function that plays the save command
 */
int play_save(Board *board, Game *game, char *path) {
	FILE *fd;
	int is_save = FALSE;

	if (game->game_mode == EDIT_MODE) {
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

/* 
 * function that checks if generate got legal parameters.
 * x is the number of cells to insert, y is number of cells to leave on board
 */
int check_generate_valid(Board *board, int x, int y) {
	int n = board->n, num_filled_cells = 0, unfilled_cells = 0;

	num_filled_cells = count_num_of_filled_cells(board);
	unfilled_cells = (n*n - num_filled_cells);

	if (x > unfilled_cells || x < 0 || y > unfilled_cells || y < 0) {
		print_err_value_not_int_range_E(unfilled_cells);
		return NOT_VALID;
	}

	else if (num_filled_cells != 0) {
 		print_err_board_not_empty();
		return NOT_VALID;
	}

	return VALID;
}

/* 
 * function that gets a row and column and randomly choose a legal value to fill in the cell
 * */
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

/*
 * function that receives a pointer to a board struct and an int X and fills the (empty) board to have exactly X filled cells
 */
int fill_x_empty_cells(Board *board, int x) {
	int n = board->n;
	int counter = 0;
	int rand_row, rand_col, rand_val;

	while (counter != x) {
		rand_row = rand() % n;
		rand_col = rand() % n;
		if (board->game_table[rand_row][rand_col].val == UNASSIGNED) {
			rand_val = get_random_legal_val(board, rand_row, rand_col);
			if (rand_val == NOT_POSSIBLE_VAL) {
				/* didn't find a legal value for the cell */
				return NOT_VALID;
			}
			board->game_table[rand_row][rand_col].val = rand_val;
		}
		counter += 1;
	}
	return VALID;
}

/*
 * function that recieves a pointer to a board struct and an int num and clears num cells from the board
 */
int remove_num_cells(Board *board, int num) {
	int rand_row = 0, rand_col = 0;
	while (num > 0) {
		rand_row = rand() % board->n;
		rand_col = rand() % board->n;
		if (board->game_table[rand_row][rand_col].val != UNASSIGNED) {
			board->game_table[rand_row][rand_col].val = UNASSIGNED;
			board->game_table[rand_row][rand_col].is_err = FALSE;
			board->game_table[rand_row][rand_col].is_fixed = FALSE;
			num -= 1;
		}

	}
	return VALID;
}

/*
 * function that plays the generate command
 */
int play_generate(Game *game, Board *board, int cells_to_fill, int cells_to_keep){
	Board *iter_board = NULL;
	Cell_Item *x_cells_arr = NULL;
	int iter;
	int cells_to_remove = board->n * board->n - cells_to_keep;
	MoveInfo move;

 	if (check_generate_valid(board, cells_to_fill, cells_to_keep) == NOT_VALID) {
 		return NOT_VALID;
	}

 	init_empty_game_move_info(&move);
 	for (iter = 0; iter < MAX_ITER_GENERATE; iter++) {
 		iter_board = init_board(board->n, board->m_rows, board->m_cols, board->filled);
 		if (copy_board(board->game_table, iter_board->game_table, board->n) != VALID) {
 			printf("Error: couldn't copy to iter board - generate\n");
 			return NOT_VALID;
 		}
 		if ((fill_x_empty_cells(iter_board, cells_to_fill) == VALID) &&
 			run_ILP(iter_board, RUN_GENERATE, 0, 0) == VALID) {
			/* generated full board */

			update_errors_on_board(iter_board);
			if (remove_num_cells(iter_board, cells_to_remove) != VALID) {
				printf("Error: couldn't remove %d cells\n", cells_to_remove);
				return NOT_VALID;
			}
			if (create_generate_chain(&move, board, iter_board) != VALID) {
				printf("Error: couldn't create chain of generate moves\n");
				return NOT_VALID;
			}

			if (append_new_move_to_moves_list(game->curr_move, move) != VALID) {
				printf("Error: couldn't add generate move to chain of game\n");
				return NOT_VALID;
			}

			game->curr_move = game->curr_move->next;
			copy_board(iter_board->game_table, board->game_table, board->n);
			free_board(iter_board);
			print_board(board, game->mark_err, game);
			free(x_cells_arr);
			return VALID;
		}
 		free_board(iter_board);
 		/*clean_vals_from_board(board);*/
	}

	print_err_generator_failed();
	clean_vals_from_board(board);
	free(x_cells_arr);
	return NOT_VALID;
}

/*
 * function that plays the validate command
 */
int play_validate(Board *board, int to_print) {
	if (board->num_err != 0) {
		if (to_print == 1){print_errorneous_err();}
		return NOT_VALID;
	}

	if (run_ILP(board, RUN_VALIDATE, 0, 0) == NOT_VALID) {
		if(to_print == 1){print_validation_failed();}
		return NOT_VALID;
	}
	else{
		if(to_print == 1){print_validation_passed();}
		return VALID;
	}
	return VALID;
}

/*
 * function that free the memory allocated for a game struct
 */
int free_game(Game *game) {
 	if (remove_moves_from_begining(&(game->curr_move)) != VALID) {
 		return NOT_VALID;
	}

	return VALID;
}

/*
 * function that plays the undo command
 */
int play_undo(Board *board, Game *game) {
	MoveInfo move_to_change;
	init_empty_game_move_info(&move_to_change);

	if (game->curr_move->prev == NULL) {
			/* this is the first move in the game */
			print_err_no_moves_to_undo();
			return NOT_VALID;
	}

	move_to_change = game->curr_move->move;
	if (undo_on_board(board, game->curr_move->move) != VALID) {
		printf("Error: couldn't make undo on board\n");
		return NOT_VALID;
	}

	print_board(board, game->mark_err, game);
	print_undo_move(move_to_change);

	game->curr_move = game->curr_move->prev;
	return VALID;
}

/*
 * function that plays the redo command
 */
int play_redo(Board *board, Game *game) {
	MoveInfo move_to_change;
	init_empty_game_move_info(&move_to_change);
	if (game->curr_move->next == NULL) {
		/* this is the first move in the game */
		print_err_no_moves_to_redo();
		return NOT_VALID;
	}

	move_to_change = game->curr_move->next->move;
	if (redo_on_board(board, game->curr_move->next->move) != VALID) {
		printf("Error: couldn't make undo on board\n");
		return NOT_VALID;
	}

	print_board(board, game->mark_err, game);
	print_redo_move(move_to_change);

 	game->curr_move = game->curr_move->next;
 	return VALID;
}

/*
 * function that receives a string, pointer to a board struct and an indication use_fixed and reads a sudoku from a file
 * who's path is the input string into the board struct. according to use_fixed indicator, mark cells as fixed or not
 */
int read_sudoku(char *path, Board *board, int use_fixed){
	int row, col, n, count;
	FILE *input;
	char read[2] = {'\0', '\0'};

	input = fopen(path, "r+");
	if (input == NULL){
		printf("problem with file\n");
		return NOT_VALID;
	}

	free_matrix(board->game_table, board->n); /* free the current game table */

	/* go over spaces until we find dimensions of sub block */
	while (isspace(read[0] = fgetc(input))){}
	row = string_to_int(read);
	while (isspace(read[0] = fgetc(input))){}
	col = string_to_int(read);
	n = col * row;

	/* update current board with new dimensions and create new empty game table */
	board->m_rows = row;
	board->m_cols = col;
	board->n = n;
	init_game_table(board);

	/* iterate on the file to update the new board */
	count = 0;
	row = 0;
	while(row < n){ /* go over rows */
		col = 0;
		while(col < n){ /* go over columns */
			board->game_table[row][col].is_err = 0;
			board->game_table[row][col].is_fixed = 0;
			if(!isspace(read[0] = fgetc(input))){ /* found a value or a dot */
				if (read[0] != '.'){ /* found a value or first digit of the value of a cell */
					board->game_table[row][col].val = string_to_int(read);
					if (board->game_table[row][col].val != 0){ /* increment filled cells */
						count += 1;
					}
					if (isspace(read[0] = fgetc(input))){ /* unfixed value */
						col += 1;
					}
					else if (read[0] != '.'){ /* double digit value in the cell */
						board->game_table[row][col].val *= 10;
						board->game_table[row][col].val += string_to_int(read);
						if (isspace(read[0] = fgetc(input))){ /* unfixed value of double digit */
							col += 1;
						}
						else{ /* fixed double digit value */
							if (use_fixed){ /* fixed is relevant only in SOLVE MODE */
								board->game_table[row][col].is_fixed = 1;
							}
							col += 1;
						}
					}
					else{ /* a dot of a fixed cell */
						if (use_fixed){ /* fixed is relevant only in SOLVE MODE */
							board->game_table[row][col].is_fixed = 1;
						}
						col += 1;
					}
				}
				else{ /* a fixed cell */
					if (use_fixed){ /* fixed is relevant only in SOLVE MODE */
						board->game_table[row][col].is_fixed = 1;
					}
					col += 1;
				}
			}
		}
		row += 1;
	}

	board->filled = count; /* updating the number of filled cells */
	update_errors_on_board(board); /* updating the error values of cells and board */
	fclose(input);
	return VALID;
}

/*
 * function that plays the solve command
 */
int play_solve(Board *board, char *path, Game *game){
	if (read_sudoku(path, board, TRUE) == NOT_VALID){ /* trouble opening the file */
		printf("Error: File doesn't exist or cannot be opened\n");
		return NOT_VALID;
	}

	if (remove_moves_from_begining(&(game->curr_move)) != VALID){
		return NOT_VALID;
	}
	game->game_mode = SOLVE_MODE;
	print_board(board, game->mark_err, game);
	return VALID;
}

/*
 * function that plays the edit command
 */
int play_edit(Board *board, char *path, Game *game){
	int col, row, n;

	if (path == NULL || path[0] == '\0'){ /* path wasn't provided */
		free_matrix(board->game_table, board->n); /* clean old board */
		board->m_cols = 3;
		board->m_rows = 3;
		board->n = 9;
		board->filled = 0;
		board->num_err = 0;
		init_game_table(board); /* create new empty board */
		n = board->n;
		for (row = 0; row < n; row++){
			for (col = 0; col < n; col++){
				board->game_table[row][col].val = 0;
				board->game_table[row][col].is_fixed = 0;
				board->game_table[row][col].is_err = 0;
			}
		}
		if (remove_moves_from_begining(&(game->curr_move)) != VALID){
			return NOT_VALID;
		}
		game->game_mode = EDIT_MODE;
		print_board(board, 1, game);
		return VALID;
	}
	else if(read_sudoku(path, board, FALSE) == NOT_VALID){ /* reading the file failed */
		printf("Error: File cannot be opened\n");
		return NOT_VALID;
	}
	else{ /* sudoku updated from file */
		if (remove_moves_from_begining(&(game->curr_move)) != VALID){
			return NOT_VALID;
		}
		game->game_mode = EDIT_MODE;
		print_board(board, 1, game);
		return VALID;
	}
}

/*
 * function that plays the exit command
 */
int play_exit(Board *board, Game *game){
	free_game(game);
	free_board(board);
	printf("Exiting...\n");
	return VALID;
}

/* 
 * functions that runs all posible "undo" in the game.
 */
int play_all_undo(Board *board, Game *game, MoveInfo *undo_move) {
	while (game->curr_move->prev != NULL) {
		*undo_move = game->curr_move->move;
		if (undo_on_board(board, game->curr_move->move) != VALID) {
			printf("Error: couldn't make undo on board\n");
			return NOT_VALID;
		}

		game->curr_move = game->curr_move->prev;

	}

	return FINISHED_UNDO;

}

/*
 * function that plays the reset command
 */
int play_reset(Board *board, Game *game) {
	MoveInfo move;

	init_empty_game_move_info(&move);
	if (play_all_undo(board, game, &move) != FINISHED_UNDO) {
		printf("Error: while play_reset, can't play undo\n");;
		return NOT_VALID;
	}

	if (remove_next_moves(game->curr_move) != VALID) {
		printf("Error: couldn't remove moves from this point in play_reset\n");
		return NOT_VALID;
	}

	print_board_reset();
	return VALID;
}

/*
 * function that checks if the game is solved and prints content accordingly
 */
void check_win(Board *board, Game *game){
	int cnt;

	cnt = count_num_of_filled_cells(board);
	if (cnt == (board->n * board->n)){ /* board is full */
		if(play_validate(board, FALSE)){
			/* validation passed */
			printf("Puzzle solved successfully\n");
			game->game_mode = INIT_MODE;
		}
		else{ /* validation failed */
			printf("Puzzle solution erroneous\n");
		}
	}
}

/*
 * function that manages the game, generates the game, board and command structs and plays each turn according
 * to the input commands from the user
 */
void play(){
	Game game;
	Board *board;
	MoveList initial_move;
	struct Command command;

	/* initializing the game */
	game.game_mode = INIT_MODE;
	game.mark_err = TRUE;
	init_empty_game_move_info(&(initial_move.move));
	initial_move.next = NULL;
	initial_move.prev = NULL;
	game.curr_move = &initial_move;

	board = init_board(9, 3, 3, 0); /* initialize a generic board */

	printf("Sudoku\n");
	printf("------\n");

	while (TRUE){ /* keep playing */
		printf("Enter your command:\n");
		command = get_command();
		if (command.valid == 0){ /* invalid command */
			printf("ERROR: invalid command\n");
		}
		else if(game.game_mode == INIT_MODE){ /* init mode */
			switch (command.command){
			case SOLVE:
				play_solve(board, command.path, &game);
				clear_path(command.path);
				break;
			case BLANK_LINE:
				break;
			case EDIT:
				play_edit(board, command.path, &game);
				break;
			case EXIT:
				play_exit(board, &game);
				return;
			default:
				printf("ERROR: invalid command\n");
				break;
			}
		}
		else if(game.game_mode == SOLVE_MODE){ /* solve mode */
			switch (command.command){
				case SOLVE:
					play_solve(board, command.path, &game);
					clear_path(command.path);
					break;
				case BLANK_LINE:
					break;
				case EDIT:
					play_edit(board, command.path, &game);
					break;
				case EXIT:
					play_exit(board, &game);
					return;
				case MARK_ERRORS:
					play_mark_errors(&game, command.X);
					break;
				case PRINT_BOARD:
					print_board(board, game.mark_err, &game);
					break;
				case SET:
					if (play_set(command, board, &game) == VALID){
						check_win(board, &game);
					}
					break;
				case VALIDATE:
					play_validate(board, TRUE);
					break;
				case UNDO:
					play_undo(board, &game);
					break;
				case REDO:
					play_redo(board, &game);
					break;
				case SAVE:
					play_save(board, &game, command.path);
					clear_path(command.path);
					break;
				case HINT:
					play_hint(command, board);
					break;
				case NUM_SOLUTIONS:
					play_num_solutions(board);
					break;
				case AUTOFILL:
					play_autofill(&board, &game, TRUE);
					check_win(board, &game);
					break;
				case RESET:
					play_reset(board, &game);
					break;
				default:
					printf("ERROR: invalid command\n");
					break;
			}
		}
		else{ /* edit mode */
			switch (command.command){
				case SOLVE:
					play_solve(board, command.path, &game);
					clear_path(command.path);
					break;
				case BLANK_LINE:
					break;
				case EDIT:
					play_edit(board, command.path, &game);
					break;
				case EXIT:
					play_exit(board, &game);
					return;
				case PRINT_BOARD:
					print_board(board, TRUE, &game);
					break;
				case SET:
					play_set(command, board, &game);
					break;
				case GENERATE:
					play_generate(&game, board, command.X, command.Y);
					break;
				case VALIDATE:
					play_validate(board, TRUE);
					break;
				case UNDO:
					play_undo(board, &game);
					break;
				case REDO:
					play_redo(board, &game);
					break;
				case SAVE:
					play_save(board, &game, command.path);
					clear_path(command.path);
					break;
				case NUM_SOLUTIONS:
					play_num_solutions(board);
					break;
				case RESET:
					play_reset(board, &game);
					break;
				default:
					printf("ERROR: invalid command\n");
					break;
			}
		}
	}
	free_board(board);
}
