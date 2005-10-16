#include <allegro.h>
#include <stdio.h>

#include "gameState.h"

int main(int argc, char* argv) {
				// process command line arguments
				// options[] = process_args(argc, argv);
				
				// run the game
				GameState gameState;
				return gameState.RunGame(/* options */);
} END_OF_MAIN();
