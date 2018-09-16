/* main module - runs the game */
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "MainAux.h"
#include "Parser.h"


int main(){
	srand(time(NULL));
	play();
	return 1;
}
