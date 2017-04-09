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

class gps_position
{
	friend class boost::serialization::access;
	friend std::ostream & operator<<(std::ostream &os, const gps_position &gp);

	int degrees;
	int minutes;
	float seconds;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */) {
		ar  & BOOST_SERIALIZATION_NVP(degrees)
			& BOOST_SERIALIZATION_NVP(minutes)
			& BOOST_SERIALIZATION_NVP(seconds);
	}

public:
	// every serializable class needs a constructor
	gps_position() {};
	gps_position(int _d, int _m, float _s) :
		degrees(_d), minutes(_m), seconds(_s)
	{}
};

#include "objectStatic.h"

void junk() {
	// create and open a character archive for output

	// create class instance
	StaticObject g;
	g.junk_test = 88888;
	g.SetXY(32, 42);

	const char* filename = "test-save.xml";

	// save data to archive
	{
		std::ofstream ofs(filename);
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(g);
	}

	// ... some time later restore the class instance to its orginal state
	StaticObject newg;
	{
		// create and open an archive for input
		std::ifstream ifs(filename);
		boost::archive::xml_iarchive ia(ifs);
		ia >> BOOST_SERIALIZATION_NVP(newg);
	}

	int x = 0;
}

int main(int argc, const char** argv) 
{
	junk();
	return 0;

	#ifdef REDIRECT_STDERR
	TRACE("Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}
	
	TRACE("Main: redirected output.\n");
	#endif

	return start_game_instance(argc, argv);

}
