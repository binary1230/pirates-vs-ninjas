#include "luaManager.h"
#include "luaApi.h"
#include "assetManager.h"

// EXPERIMENTAL: I'm using lua version 5.0, should be ok with lua 5.1?

DECLARE_SINGLETON(LuaManager);

LuaManager::LuaManager() {lua=NULL;}
LuaManager::~LuaManager() {lua=NULL;}

bool LuaManager::Init() {
  lua = lua_open();

  if (NULL == lua)
  {
     printf("LUA: Error Initializing lua VM!\n");
     return false;
  }

  // initialize lua standard library functions

  lua_baselibopen(lua);
  lua_iolibopen(lua);
  lua_strlibopen(lua);
  lua_mathlibopen(lua);
	
	if (!BindLuaCalls()) {
		fprintf(stderr, "LUA: Failed to bind C++ calls to Lua!\n");
		return false;
	}
	
	return true;
}

// slight hack to do this for now when all we really want 
// to do is clear the LUA state itself.
void LuaManager::Clear() {
	Shutdown();
	Init();
}

// Example demonstrating how to call a LUA function from
// C++ with no arguments that returns an int
// Returns true if everything went OK.
bool LuaManager::RunVoidFunctionNoArgs(	const char* functionName, 
																				bool errorMsgOnFunctionNotFound) {

	const int numArgs = 0;	
	const int numResults = 0;

	lua_getglobal(lua, functionName);

	// not using args right now
  //lua_pushnumber(lua, value1);   // push 1st argument 
  //lua_pushnumber(lua, value2);   // push 2nd argument 

  if (lua_pcall(lua, numArgs, numResults, 0) != 0) {
		if (errorMsgOnFunctionNotFound)
			fprintf(stderr, "LUA: Error running function `%s': %s", 
							functionName, lua_tostring(lua, -1));
		return false;
	}

	// retrieve result 
  // if (!lua_isnumber(lua, -1))
	//   fprintf(stderr, "function `add_two_things' must return a number");

	// get the return value
  // int result = lua_tonumber(lua, -1);
  // lua_pop(lua, 1);

	return true;
}

// run the specified LUA script file
bool LuaManager::DoFile(const char* _filename) {
	CString filename = ASSETMANAGER->GetPathOf(_filename);

	if (!filename.length()) {
		fprintf(stderr, "ERROR: Can't open LUA file '%s'\n", _filename);
		return false;
	}

	if (lua_dofile(lua, filename.c_str()) != 0)
		return false;

	return true;
}

void LuaManager::Shutdown() {
	lua_close(lua);
}

bool LuaManager::BindLuaCalls() {
	const char* lua_function_name;
	int LUAAPI (*cpp_function)(lua_State*);
	int i = 0;

	struct LuaApiFunction* LuaApiFunctionList = GetLuaApiFunctionList();

	while (1) {
		lua_function_name = LuaApiFunctionList[i].lua_function_name;
		cpp_function = LuaApiFunctionList[i].cpp_function;

		if (!lua_function_name)
			break;

		lua_register(lua, lua_function_name, cpp_function);
		++i;
	}

	return true;
}
