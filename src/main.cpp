#include <allegro.h>
#include <stdio.h>

#include "gameState.h"
#include "gameOptions.h"

int main(int argc, char* argv[]) {
				
				// see if our command line args are OK
				GameOptions options;
				options.ParseArguments(argc, argv);
				options.PrintUsage(argv[0]);

				if (!options.IsValid())	{
					return -1;
				} else {
					GameState gameState;
					return gameState.RunGame(&options);
				}

} END_OF_MAIN();
