#include "stdafx.h"
#include "gameState.h"

#include "gameOptions.h"
#include "input.h"
#include "window.h"
#include "gameMode.h"
#include "assetManager.h"
#include "xmlParser.h"
#include "gameSound.h"
#include "gameModes.h"
#include "globalDefines.h"
#include "luaManager.h"
#include "physics.h"
#include "gameWorld.h"
#include "objectFactory.h"

DECLARE_SINGLETON(GameState)

// Parse the master XML file
// returns: XMLNode of first GameMode to load
int GameState::LoadXMLConfig(std::string xml_filename) {
				
	string xml_path = ASSETMANAGER->GetPathOf(xml_filename.c_str());
	if (xml_path.length() <= 0) {
		TRACE("Error: GameState can't find mater XML file named %s", xml_filename.c_str());
		return -1;
	}

	xGame = XMLNode::openFileHelper(xml_path.c_str(), "game");
	
	XMLNode xInfo = xGame.getChildNode("info");

	TRACE(
		" Mod Info: requires engine version '%s'\n"
		" Mod Info: map version '%s'\n"
		" Mod Info: map author '%s'\n"
		" Mod Info: Description: '%s'\n",
		// " Mod Info: Number of modes: '%i'\n"
		xInfo.getChildNode("requires_engine_version").getText(),
		xInfo.getChildNode("game_version").getText(),
		xInfo.getChildNode("author").getText(),
		xInfo.getText()
		//max, 
		);

	// Init globals XML stuff
	XMLNode xGlobalVars = xGame.getChildNode("global_vars");
	GLOBALS->CreateInstance();
	GLOBALS->Init(xGlobalVars);

	return 0;	
}

void GameState::SignalEndCurrentMode() {
	modes->SignalEndCurrentMode();
}

void allegro_debug_printer(const char *text)
{
	// filter out some spam here.
	if (strncmp("agl-tex INFO", text, 12) != 0 &&
		strncmp("agl-font INFO", text, 13) != 0 &&
		strncmp("agl-win INFO", text, 12) != 0 &&
		strncmp("agl-scorer INFO", text, 15) != 0 &&
		strncmp("al-gfx INFO", text, 11) != 0 )
	{
		        
		TRACE("%s", text);
	}
}

//! Initialize basic allegro library stuff
//! This must be called FIRST before ANY allegro stuff
int GameState::InitAllegro() {
	
	#if ALLEGRO_VERBOSE_DEBUG_OUTPUT
	al_register_trace_handler(allegro_debug_printer);
	#endif

	if (!al_init()) {
		TRACE("-- FATAL ERROR: Allegro_init() failed.\n");
		return -1;
	}

	SetRandomSeed(42);	// for now, makes testing easier

	return 0;
}

//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the default game mode
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystems() {
		
	TRACE("[Beginning Game Init]\n");
				
	exit_game = false;
	is_playing_back_demo = false;
	paused = false;
	unpause_only_this_frame = false;
	should_redraw = true;

	RegisterObjectPrototypes();

	TRACE("[init: allegro]\n");
	if (InitAllegro() < 0) {
		TRACE("ERROR: InitSystems: failed to init allegro!\n");
		return -1;
	}

	TRACE("[init: assetManager]\n");
	ASSETMANAGER->CreateInstance();
	if (!ASSETMANAGER || ASSETMANAGER->Init() < 0) {
		TRACE("ERROR: InitSystems: failed to create assetManager!\n");
		return -1;
	}

	ASSETMANAGER->AppendToSearchPath("data/");			// what we normally expect
	ASSETMANAGER->AppendToSearchPath("../data/");		// for debugging in Visual studio
	ASSETMANAGER->AppendToSearchPath("../../data/");		// for debugging in Visual studio

	TRACE("[init: xml config]\n");

	// just DIES if it can't load this file (bad)
	if (LoadXMLConfig("default.xml") < 0) {
		TRACE("ERROR: Failed to parse default.xml");	
		return -1;
	}

	TRACE("[init: window]\n");
	WINDOW->CreateInstance();
	if ( !WINDOW ||	WINDOW->Init(
		screen_size_x, screen_size_y, OPTIONS->IsFullscreen(), OPTIONS->GraphicsMode()) < 0 ) {
		TRACE("ERROR: InitSystems: failed to init window!\n");
		return -1;
	}

	TRACE("[init: input subsystem]\n");
	if (InitInput() == -1) {
		TRACE("ERROR: InitSystems: failed to init input subsystem!\n");
		return -1;
	}

	TRACE("[init: sound subsystem]\n");
	if (InitSound() == -1) {
		TRACE("ERROR: InitSystems: failed to init sound subsystem!\n");
	}

	TRACE("[init: embedded lua scripting]\n");
	LUA->CreateInstance();
	if ( !LUA || !LUA->Init() ) {
		TRACE("ERROR: InitSystems: failed to init lua scripting!\n");
		return -1;
	}

	TRACE("[init: loading game modes]\n");
	if (LoadGameModes() == -1) {
		TRACE("ERROR: InitSystems: failed to init default game mode!\n");
		return -1;
	}

	if (!InitAllegroEvents()) {
		TRACE("ERROR: InitSystems: Can't init timers.\n");
		return -1;
	}
		
	TRACE("[init complete]\n");
				
	return 0;
}

int GameState::LoadGameModes() {
	modes = new GameModes();

	if (!modes)
		return -1;

	return modes->Init(xGame);
}

//! Init sound subsystem
//TODO if sound init fails, make it just keep going instead of erroring out.
int GameState::InitSound() {

	SOUND->CreateInstance();

	if (!SOUND) {
		TRACE(" Failed to create sound instance.\n");
		return -1;
	}

	if (!OPTIONS->SoundEnabled())
		TRACE(" Sound disabled.\n");

	if ( !SOUND || (SOUND->Init(OPTIONS->SoundEnabled()) == -1) ) {
		return -1;
	}
				
	return 0;
}

//! Init input subsystem
int GameState::InitInput() {
	INPUT->CreateInstance();
	
	if ( !INPUT || (INPUT->Init() == -1) ) {
		return -1;
	}

	return 0;
}

//! Init game timers
//! This MUST be called BEFORE any other allegro initializations.
bool GameState::InitAllegroEvents() {
	TRACE("[Init: Timers/Events]");
	
	event_queue = al_create_event_queue();
	if (!event_queue) {
		TRACE("failed to create event_queue!\n");
		return false;
	}

	m_timer = al_create_timer(ALLEGRO_BPS_TO_SECS(FPS));
	if (!m_timer) {
		TRACE("failed to create timer!\n");
		return false;
	}

	al_register_event_source(event_queue, al_get_timer_event_source(m_timer));
	al_register_event_source(event_queue, al_get_display_event_source(WINDOW->GetDisplay()));

	al_start_timer(m_timer);

	return true;
}

bool GameState::Init(const int argc, const char** argv) {
	#ifdef REDIRECT_STDERR
	TRACE("Redirecting stderr output to '" REDIRECT_STDERR_FILENAME "'\n");

	if (!freopen(REDIRECT_STDERR_FILENAME, "wt", stderr)) {
		printf("Couldn't redirect stderr to "REDIRECT_STDERR_FILENAME "!");
	}

	TRACE("Main: redirected output.\n");
	#endif

	if (!OPTIONS) {
		OPTIONS->CreateInstance();
		OPTIONS->ParseArguments(argc, argv);
	}

	if (!OPTIONS->IsValid()) {
		TRACE("ERROR: Failed to init game - invalid commandline args!\n");
		return false;
	}

	if (InitSystems() == -1) {
		TRACE("ERROR: Failed to init game!\n");
		return false;
	}

	if (OPTIONS->GetDebugStartPaused())
		paused = 1;

	INPUT->Begin();

	TRACE("[running game...]\n");
	return true;
}

void GameState::RunMainLoop_BlockingHelper()
{
	while (!ShouldExit()) {
		ProcessEvents();
		TickIfNeeded();
	}
}

void GameState::ProcessEvents() 
{
	ALLEGRO_EVENT ev;
	while (al_get_next_event(event_queue, &ev)) {
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			// important note: this event, on mode loads, may trigger dozens of times 
			// because the timer was triggering during the loads.
			// we handle this in a way that is able to ignore those queued up extra ticks so the game
			// doesn't run fast until we're caught up.
			should_redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exit_game = true;
		}
	}
}

void GameState::TickIfNeeded() {
	if (!should_redraw)
		return;

	should_redraw = false;
	
	Tick();
}

void GameState::Tick() {
	Update();
	Draw();
}

void GameState::UpdateGlobalInput()
{
	if (INPUT->KeyOnce(GAMEKEY_DEBUGPAUSE))
		SetPaused(!IsPaused());

	unpause_only_this_frame = false;
	if (paused && INPUT->KeyOnce(GAMEKEY_DEBUGSTEP)) {
		unpause_only_this_frame = true;
		paused = false;
	}

	if (INPUT->KeyOnce(GAMEKEY_SCREENSHOT))
		WINDOW->Screenshot();

	if (INPUT->KeyOnce(GAMEKEY_TOGGLE_PHYSICS_DISPLAY))
		SetPhysicsDebugDraw(!GetPhysicsDebugDraw());

	if (INPUT->KeyOnce(GAMEKEY_SAVE_MAP))
		WORLD->SaveWorldOverCurrentFile();
}

//! Update all game status
void GameState::Update() {
	if (exit_game)
		return;

	UpdateGlobalInput();

	SOUND->Update();
	INPUT->Update();

	modes->Update();
	WINDOW->Update(); // update fades

	if (unpause_only_this_frame) {
		unpause_only_this_frame = false;
		paused = true;
	}
}

void GameState::Draw() {
	if (exit_game || !OPTIONS->DrawGraphics())
		return;

	WINDOW->BeginDrawing();
	WINDOW->Clear();

	modes->Draw();
	WINDOW->Draw();

	WINDOW->Flip();
	WINDOW->EndDrawing();
}

void GameState::Shutdown() {
	TRACE("[Shutting Down]\n");

	if (m_timer) {
		al_destroy_timer(m_timer);
		m_timer = NULL;
	}

	if (INPUT) {
		INPUT->End();
		INPUT->Shutdown();
		INPUT->FreeInstance();
	}

	if (event_queue)
		al_destroy_event_queue(event_queue);
	
	if (modes) {
		modes->Shutdown();
		delete modes;
	}

	if (LUA) {
		LUA->Shutdown();
	}

	if (SOUND) {
		SOUND->Shutdown();
		SOUND->FreeInstance();
	}

	if (ASSETMANAGER) {
		ASSETMANAGER->Shutdown();
		ASSETMANAGER->FreeInstance();
	}
	
	// window destruction code must be LAST
	if (WINDOW) {
		WINDOW->Shutdown();
		WINDOW->FreeInstance();
	}

	if (GLOBALS)
		GLOBALS->Shutdown();

	if (OPTIONS)
		OPTIONS->FreeInstance();

	modes = NULL;
	network = NULL;
	xGame = XMLNode::emptyXMLNode;
}

void GameState::SetRandomSeed(int val) { 
	random_seed = val; 
	srand(val); 
};

int GameState::GetRandomSeed() const { 
	return random_seed; 
};

GameState::GameState() {
	modes = NULL;
	network = NULL;
	m_timer = NULL;
	_PhysicsDebugDraw = false;
}

void GameState::SignalGameExit() {
	exit_game = true; 
	is_playing_back_demo = false;
	modes->SignalGameExit();
}

GameState::~GameState() {}