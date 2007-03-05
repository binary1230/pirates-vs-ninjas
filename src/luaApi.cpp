#include "luaApi.h"

int LUAAPI lua_engine_print(lua_State* lua) {
	const char* lua_argument1 = lua_tostring(lua, -1);

	printf("%s", lua_argument1);

  lua_pushnumber( lua, 0 );
  return 1;
}

static struct LuaApiFunction LuaApiFunctionList[] = {

	// first item: function name as it appears from lua
	// second item: our corresponding C++ function
	
	{ "engine_print", lua_engine_print },

	// terminating null entry
	{ 0, 0 } 
};

struct LuaApiFunction* GetLuaApiFunctionList() {
	return LuaApiFunctionList;
}
