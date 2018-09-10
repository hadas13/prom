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

/*int main() {
	Board *board;
	Game *game;
	struct Command cmd;
	int n = 16;
	int m_rows = 4;
	int m_cols = 4;
	cmd.X = 1;
	cmd.Y = 1;
	cmd.Z = 3;
	board = init_board(n, m_rows, m_cols, 0);
	game = init_game();
	game->game_mode = SOLVE;
	play_generate(game,  board, 10, 6);
	read_sudoku("in", board);
	print_board(board, TRUE);

	play_num_solutions(board);

	play_save(board, game, "here.txt");

	print_board(board, TRUE);
	free_board(board);
	free_game(game);
	return 1;
}*/

int main(){
	play();
	return 1;
}
