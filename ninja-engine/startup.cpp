#include "startup.h"
#include "startup.h"
#include "startup.h"
#include "stdafx.h"
#include "startup.h"
#include "gameState.h"
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

DllExport bool ninjas_engine_init(const int argc, const char* argv[]) {
	assert(!GAMESTATE);
	GAMESTATE->CreateInstance();
	return GAMESTATE->Init(argc, argv);
}

DllExport void ninjas_engine_shutdown() {
	assert(GAMESTATE);
	GAMESTATE->Shutdown();
	GAMESTATE->FreeInstance();
}

DllExport void ninjas_engine_run__blocking_helper() {
	assert(GAMESTATE);
	GAMESTATE->RunMainLoop_BlockingHelper();
}

DllExport bool ninjas_engine_should_exit()
{
	assert(GAMESTATE);
	return GAMESTATE->ShouldExit();
}

DllExport void ninjas_engine_process_events() {
	assert(GAMESTATE);
	GAMESTATE->ProcessEvents();
}

DllExport void ninjas_engine_tick() {
	assert(GAMESTATE);
	GAMESTATE->TickIfNeeded();
}
