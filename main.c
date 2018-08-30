#include "SPBufferset.h"
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "MainAux.h"
#include "Parser.h"

/*int main(int argc, char **argv){*/
	/*int seed;*/
	
	/*SP_BUFF_SET();*/
	/*if (argc != 2) {*/
		/*[> didn't insert seed <]*/
		/*seed = 5;*/
	/*}*/
	/*else {*/
		/*seed = string_to_int(argv[argc-1]); [> get the seed int value <]*/
	/*}*/
	/*srand(seed);*/
	/*play();*/
	/*return(0);*/
/*}*/

int main() {
	Board *board;
	Game *game;
	int n = 4;
	int m_rows = 2;
	int m_cols = 2;
	board = init_board(n, m_rows, m_cols, 0);
	game = init_game();
	printf("this is : %d\n", game->mark_err);
	board->game_table[1][1].val = 1;
	board->game_table[0][0].val = 3;
	board->game_table[0][1].val = 4;
	board->game_table[2][1].val = 2;
	board->game_table[3][0].val = 2;
	print_board(board, TRUE);
	board = autofill(board, TRUE);

	play_save(board, game, "here.txt");

	print_board(board, TRUE);
	free_board(board);
	return 1;
}
