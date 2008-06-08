#include "stdafx.h"
#include "gameState.h"
#include "gameOptions.h"
#include "globals.h"

// #define REDIRECT_STDERR 1
#define REDIRECT_STDERR_FILENAME "/Users/dcerquetti/game.log"

int start_game_instance(const int argc, const char* argv[]) {

	int ret_val = -1;
	
	OPTIONS->CreateInstance();
	assert(OPTIONS);

	OPTIONS->PrintBanner();
	OPTIONS->ParseArguments(argc, argv);
	OPTIONS->PrintOptions(argv[0]);

	if (OPTIONS->IsValid())	{
		GAMESTATE->CreateInstance();
		assert(GAMESTATE != NULL && "ERROR: Can't create gamestate instance!\n");

		ret_val = GAMESTATE->RunGame();
	}

	GAMESTATE->FreeInstance();
	OPTIONS->FreeInstance();

	return ret_val;
}

/// The main function
/// IT ROCKS
int main(const int argc, const char* argv[]) 
{
	#ifdef REDIRECT_STDERR
	TRACE("Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}
	
	TRACE("Main: redirected output.\n");
	#endif

	return start_game_instance(argc, argv);

} END_OF_MAIN();
