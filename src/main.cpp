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

/*
void junk() {
	const char* filename = "test-save.xml";

	// save data to archive
	{
		GameMode* w = WORLD;
		std::ofstream ofs(filename);
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(w);
	}

	// ... some time later restore the class instance to its orginal state
	{
		// create and open an archive for input
		GameMode* w;
		std::ifstream ifs(filename);
		boost::archive::xml_iarchive ia(ifs);
		ia >> BOOST_SERIALIZATION_NVP(w);
	}
}
*/

int main(int argc, const char** argv) 
{
	#ifdef REDIRECT_STDERR
	TRACE("Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}
	
	TRACE("Main: redirected output.\n");
	#endif

	return start_game_instance(argc, argv);

}
