#include "SPBufferset.h"
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "MainAux.h"
#include "Parser.h"

int main(int argc, char **argv){
	int seed;
	
	SP_BUFF_SET();
	if (argc != 2) {
		/* didn't insert seed */
		seed = 5;
	}
	else {
		seed = string_to_int(argv[argc-1]); /* get the seed int value */
	}
	srand(seed);
	play();
	return(0);
}
