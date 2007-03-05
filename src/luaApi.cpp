#include "luaApi.h"
#include "gameWorld.h"
#include "objectTxtOverlay.h"

// LUA: Debug only - print something to stderr from lua
// through the engine.  Use for testing lua only really.
int LUAAPI lua_engine_print(lua_State* lua) {
	const char* msg = lua_tostring(lua, -1);
	int retval = 0;

	fprintf(stderr, "%s", msg);

  lua_pushnumber( lua, retval );
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

			WORLD->AddObject(obj, true);
		}
	}

	lua_pushnumber(lua, retval);
	return 1;
}

static struct LuaApiFunction LuaApiFunctionList[] = {

	// first item: function name as it appears from lua
	// second item: our corresponding C++ function
	
	{ "engine_print", lua_engine_print },
	{ "world_textbox", lua_world_textbox },

	// terminating null entry
	{ 0, 0 } 
};

struct LuaApiFunction* GetLuaApiFunctionList() {
	return LuaApiFunctionList;
}
