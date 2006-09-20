#include "stdafx.h"
#include "gameState.h"
#include "gameOptions.h"

#define REDIRECT_STDERR 1
#define REDIRECT_STDERR_FILENAME "/Users/dcerquetti/game.log"

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

#	ifdef REDIRECT_STDERR
	fprintf(stderr, "Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}
	
	fprintf(stderr, "Main: redirected output.\n");
# endif

	return start_game_instance(argc, argv);

} END_OF_MAIN();
