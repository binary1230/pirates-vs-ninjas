#include "stdafx.h"
#include "luaApi.h"
#include "gameWorld.h"
#include "objectText.h"
#include "input.h"
#include "objectFactory.h"
#include "objectCutBars.h"
#include "window.h"
#include "gameSound.h"
#include "game.h"
#include "gameOptions.h"

// LUA: Debug only - print something to stderr from lua
// through the engine.  Use for testing lua only really.
int LUAAPI lua_engine_print(lua_State* lua) {
	const char* msg = lua_tostring(lua, -1);
	int retval = 0;

	TRACE("%s", msg);

	lua_pushnumber( lua, retval );
	return 1;
}

int LUAAPI lua_world_create_cutbars(lua_State* lua) {
	const char* txt = lua_tostring(lua, -1);
	int retval = 0;
	
	if (!WORLD) {
		retval = -1;
	} else {
		ObjectCutBars* obj = (ObjectCutBars*)OBJECT_FACTORY->CreateObject("CutBars");
		if (!obj) {
			retval = -1;
		} else {
			if (txt)
				obj->SetText(txt);

			obj->SetLayer( WORLD->FindLayer("overlays") );
			WORLD->AddObject(obj);

			obj->Start();
		}
	}

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_world_allow_player_offscreen(lua_State* lua) {
	bool state = lua_tonumber(lua, -1) != 0;
	int retval = 0;

	if (!WORLD) {
		retval = -1;
	} else {
		WORLD->AllowPlayerOffscreen(state);
	}
	
	lua_pushnumber(lua, retval);
	return 1;
}

// LUA: Create a "textbox" with an avatar and a message.
int LUAAPI lua_world_textbox(lua_State* lua) {

	const char* txt = lua_tostring(lua, -2);
	const char* avatar_filename = lua_tostring(lua, -1);
	int retval = 0;
	
	if (!WORLD) {
		retval = -1;
	} else {
		ObjectText* obj = new ObjectText();
		if (!obj) {
			retval = -1;
		} else {
			obj->Init();

			if (txt)
				obj->SetText(txt);

			if (avatar_filename)
				obj->SetAvatarFilename(avatar_filename);

			obj->SetLayer( WORLD->FindLayer("overlays") );

			obj->SetModalActive(true);

			WORLD->AddObject(obj);
		}
	}

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_world_play_input_script(lua_State* lua) {
	const char* input_playback_filename = lua_tostring(lua, -1);
	int retval = 0;

	if (!WORLD || !INPUT)
		retval = -1;

	if (retval == 0) {
		INPUT->InitPlayback(input_playback_filename, false);
		INPUT->Begin();
	}

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_music_play(lua_State* lua) {
	const char* music_file = lua_tostring(lua, -1);
	int retval = 0;

	if (!SOUND)
		retval = -1;

	if (retval != -1 && music_file) {
		SOUND->PlayMusic(music_file);
	}

	lua_pushnumber(lua, retval);
	return 1;
}

// XXX TODO: Not implemented yet.
int LUAAPI lua_music_stop(lua_State* lua) {
	int retval = 0;

	if (!SOUND)
		retval = -1;

	if (retval != -1) {
		// SOUND->StopMusic();
	}

	lua_pushnumber(lua, retval);
	return 1;
}



int LUAAPI lua_window_fadein(lua_State* lua) {
	int rate = (int)lua_tonumber(lua, -1);
	int retval = 0;

	if (!WINDOW)
		retval = -1;
	else
		WINDOW->FadeIn(rate);

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_window_fadeout(lua_State* lua) {
	int rate = (int)lua_tonumber(lua, -1);
	int retval = 0;

	if (!WINDOW)
		retval = -1;
	else
		WINDOW->FadeOut(rate);

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_window_set_faded_out(lua_State* lua) {
	int retval = 0;

	if (!WINDOW)
		retval = -1;
	else
		WINDOW->SetFadedOut();

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_window_set_faded_in(lua_State* lua) {
	int retval = 0;

	if (!WINDOW)
		retval = -1;
	else
		WINDOW->SetFadedIn();

	lua_pushnumber(lua, retval);
	return 1;
}

// returns true if, on loading a level, we are jumping back
// from a door.  Typically can use this to not display first-time
// level opening stuff (like dialogs) when just jumping back into
// the level from somewhere.
int LUAAPI lua_jumped_back_from_a_door(lua_State* lua) {
	int retval = 0;

	if (WORLD && WORLD->JumpedBackFromADoor()) {
		retval = 1;
	}

	lua_pushnumber(lua, retval);
	return 1;
}

int LUAAPI lua_engine_should_exit_game(lua_State* lua) 
{
	int retval = GAME->ShouldExit() ? 1 : 0;
	lua_pushnumber(lua, retval);
	return 1;
}

/*int LUAAPI lua_mapeditor_toggle_grid_resolution(lua_State* lua) 
{
	if (!GetMapEditorInstance())
		return 1;

	int retval = 0;

	GetMapEditorInstance()->SetGridResolution(lua_tonumber(lua, -1));

	lua_pushnumber(lua, retval);
	return 1;
}*/

// ----------------------------------------------------------------------


#define LUA_FUNCTION_REGISTRATION_LIST_START() \
struct LuaApiFunction* GetLuaApiFunctionList() { \
	static struct LuaApiFunction LuaApiFunctionList[] = {


#define LUA_FUNCTION_REGISTRATION_LIST_END() \
	{ 0, 0 } \
	}; \
	return LuaApiFunctionList; \
}


#define REGISTER_LUA_FUNCTION(lua_function_name) \
{ #lua_function_name, lua_##lua_function_name },


// ----------------------------------------------------------------------

LUA_FUNCTION_REGISTRATION_LIST_START()
	REGISTER_LUA_FUNCTION(engine_print)
	REGISTER_LUA_FUNCTION(engine_should_exit_game)
	//REGISTER_LUA_FUNCTION(mapeditor_toggle_grid_resolution)

	REGISTER_LUA_FUNCTION(world_textbox)
	REGISTER_LUA_FUNCTION(world_play_input_script)
	REGISTER_LUA_FUNCTION(world_create_cutbars)
	REGISTER_LUA_FUNCTION(world_allow_player_offscreen)

	REGISTER_LUA_FUNCTION(window_fadeout)
	REGISTER_LUA_FUNCTION(window_fadein)
	REGISTER_LUA_FUNCTION(window_set_faded_out)
	REGISTER_LUA_FUNCTION(window_set_faded_in)

	REGISTER_LUA_FUNCTION(music_play)

	REGISTER_LUA_FUNCTION(jumped_back_from_a_door)
	REGISTER_LUA_FUNCTION(music_stop) // XXX TODO: Not implemented yet.
LUA_FUNCTION_REGISTRATION_LIST_END()
