#include "luaApi.h"
#include "gameWorld.h"
#include "objectTxtOverlay.h"
#include "input.h"
#include "objectFactory.h"
#include "objectCutBars.h"
#include "window.h"
#include "gameSound.h"

// LUA: Debug only - print something to stderr from lua
// through the engine.  Use for testing lua only really.
int LUAAPI lua_engine_print(lua_State* lua) {
	const char* msg = lua_tostring(lua, -1);
	int retval = 0;

	fprintf(stderr, "%s", msg);

  lua_pushnumber( lua, retval );
  return 1;
}

int LUAAPI lua_world_create_cutbars(lua_State* lua) {

	const char* txt = lua_tostring(lua, -1);
	int retval = 0;
	
	if (!WORLD) {
		retval = -1;
	} else {
		CutBarObject* obj = (CutBarObject*)OBJECT_FACTORY->CreateObject("CutBars");
		if (!obj) {
			retval = -1;
		} else {
			if (txt)
				obj->SetText(txt);

			obj->SetLayer( WORLD->FindLayer("overlays") );
			WORLD->AddObject(obj);
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
			obj->SetupCachedVariables();

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

int LUAAPI lua_play_input_script(lua_State* lua) {
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
		SOUND->LoadMusic(music_file);
		SOUND->PlayMusic();
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


static struct LuaApiFunction LuaApiFunctionList[] = {

	// first item: function name as it appears from lua
	// second item: our corresponding C++ function
	
	{ "engine_print", lua_engine_print },
	{ "world_textbox", lua_world_textbox },
	{ "world_play_input_script", lua_play_input_script },
	{ "world_create_cutbars", lua_world_create_cutbars },
	{ "world_allow_player_offscreen", lua_world_allow_player_offscreen },
	{ "window_fadeout", lua_window_fadeout },
	{ "window_fadein",  lua_window_fadein  },
	{ "window_set_faded_out", lua_window_set_faded_out },
	{ "window_set_faded_in",  lua_window_set_faded_in  },
	{ "music_play", lua_music_play },

	// XXX TODO: Not implemented yet.
	{ "music_stop", lua_music_stop },

	// terminating null entry
	{ 0, 0 } 
};

struct LuaApiFunction* GetLuaApiFunctionList() {
	return LuaApiFunctionList;
}
