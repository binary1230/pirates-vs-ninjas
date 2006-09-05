#include <allegro.h>
#include <stdio.h>

#include "gameState.h"
#include "gameOptions.h"
#include "StdString.h"

int start_game_instance(const int argc, const char* argv[]) {
	
	// see if our command line args are OK
	GameOptions* options = new GameOptions();
	options->PrintBanner();
	options->ParseArguments(argc, argv);
	options->PrintOptions(argv[0]);

	if (!options->IsValid())	{
		delete options;
		options = NULL;
		return -1;
	} else {
		// if OK, run the actual game
		GameState *gameState = new GameState();
		int ret_val = gameState->RunGame(options);

		delete gameState;
		gameState = NULL;

		delete options;
		options = NULL;

		return ret_val;
	}
}

/// The main function
/// IT ROCKS
int main(const int argc, const char* argv[]) {

	return start_game_instance(argc, argv);

} END_OF_MAIN();
