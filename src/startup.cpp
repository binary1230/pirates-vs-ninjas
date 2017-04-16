#include "stdafx.h"
#include "startup.h"
#include "gameState.h"
#include "gameOptions.h"
#include "globals.h"
#include "assetManager.h"

// entry point for entire game
// calling in here runs the game til we exit
DllExport int start_ninjas_engine(const int argc, const char* argv[])
{
#ifdef REDIRECT_STDERR
	TRACE("Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}

	TRACE("Main: redirected output.\n");
#endif

	int ret_val = -1;

	OPTIONS->CreateInstance();
	assert(OPTIONS);

	OPTIONS->PrintBanner();

	TRACE("Current working directory: %s", AssetManager::GetCurrentWorkingDir().c_str());
	TRACE("Current EXE Path: %s", AssetManager::GetCurrentExeFullPath().c_str());

	OPTIONS->ParseArguments(argc, argv);
	OPTIONS->PrintOptions(argv[0]);

	if (OPTIONS->IsValid()) {
		GAMESTATE->CreateInstance();
		assert(GAMESTATE != NULL && "ERROR: Can't create gamestate instance!\n");

		ret_val = GAMESTATE->RunGame();
	}

	GAMESTATE->FreeInstance();
	OPTIONS->FreeInstance();

	return ret_val;
}