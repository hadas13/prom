#include "SPBufferset.h"
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "MainAux.h"
#include "Parser.h"
#include "Moves.h"


int main() {
	MoveList initial_move;
	Board *board;
	Game game;
	struct Command cmd1, cmd2;
	int n = 4;
	int m_rows = 2;
	int m_cols = 2;
	cmd1.X = 1;
	cmd1.Y = 1;
	cmd1.Z = 3;
	cmd2.X = 2;
	cmd2.Y = 2;
	cmd2.Z = 2;

	board = init_board(n, m_rows, m_cols, 0);
	/* from here needs to be in main */
	game.game_mode = INIT_MODE;
	game.mark_err = TRUE;
	init_empty_game_move_info(&(initial_move.move));
	initial_move.next = NULL;
	initial_move.prev = NULL;
	game.curr_move = &initial_move;
	/*until here */
	/*game.game_mode = EDIT_MODE;*/
	/*play_generate(game,  board, 10, 6);*/
	play_set(cmd1, board, &game);
	play_set(cmd2, board, &game);
	play_undo(board, &game); 
	play_redo(board, &game); 

	board->game_table[2][2].val = 1;
	board->game_table[3][3].val = 4;
	board->game_table[2][3].val = 2;
	printf("here\n");
	play_autofill(&board, &game, TRUE);
	printf("this is filled: %d\n", board->filled);
	play_num_solutions(board);
	play_undo(board, &game); 
	play_redo(board, &game); 
	play_redo(board, &game); 

	free_board(board);
	free_game(&game);
	return 1;
}
