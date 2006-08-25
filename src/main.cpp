#include "allegro_wrapper.h"
#include <stdio.h>

#include "gameState.h"
#include "gameOptions.h"
#include "StdString.h"

/// The main function
/// IT ROCKS
int main(int argc, char* argv[]) {

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

} END_OF_MAIN();
