#include <allegro.h>
#include <stdio.h>

#include "gameState.h"
#include "gameOptions.h"
#include "StdString.h"

int start_game_instance(int argc, char* argv[]) {
	
	// see if our command line args are OK
	GameOptions options;
	options.PrintBanner();
	options.ParseArguments(argc, argv);
	options.PrintOptions(argv[0]);

	if (!options.IsValid())	{
		return -1;
	} else {
		// if OK, run the actual game
		GameState gameState;
		return gameState.RunGame(&options);
	}

}

/// The main function
/// IT ROCKS
int main(int argc, char* argv[]) {

	return start_game_instance(argc, argv);

} END_OF_MAIN();
