#include <allegro.h>
#include <stdio.h>

#include "gameState.h"
#include "gameOptions.h"

int main(int argc, char* argv[]) {
				
				GameOptions options;
				options.PrintUsage(argv[0]);

				options.ParseArguments(argc, argv);

				if (options.IsValid()) {
					GameState gameState;
					return gameState.RunGame(&options);
				} else {
					return -1;
				}

} END_OF_MAIN();
