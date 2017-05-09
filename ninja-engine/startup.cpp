#include "startup.h"
#include "startup.h"
#include "startup.h"
#include "stdafx.h"
#include "startup.h"
#include "game.h"
#include "gameOptions.h"
#include "globals.h"
#include "assetManager.h"

// helper game running function
// calling in here starts and runs the game til we exit.
DllExport int run_ninjas_engine___helper(const int argc, const char* argv[])
{
	int ret_val = 0;

	if (!ninjas_engine_init(argc, argv)) {
		ret_val = -1;
	} else {
		ninjas_engine_run__blocking_helper();
	}

	ninjas_engine_shutdown();

	return ret_val;
}

DllExport bool ninjas_engine_init(const int argc, const char** argv) {
	assert(!GAME);
	GAME->CreateInstance();
	return GAME->Init(argc, argv);
}

DllExport void ninjas_engine_shutdown() {
	assert(GAME);
	GAME->Shutdown();
	GAME->FreeInstance();
}

DllExport void ninjas_engine_run__blocking_helper() {
	assert(GAME);
	GAME->RunMainLoop_BlockingHelper();
}

DllExport bool ninjas_engine_should_exit()
{
	assert(GAME);
	return GAME->ShouldExit();
}

DllExport void ninjas_engine_process_events() {
	assert(GAME);
	GAME->ProcessEvents();
}

DllExport void ninjas_engine_tick() {
	assert(GAME);
	GAME->TickIfNeeded();
}

DllExport void ninjas_engine_test()
{
	TRACE("test42424242424");
}
