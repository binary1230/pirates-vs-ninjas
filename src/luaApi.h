#ifndef LUA_API_H
#define LUA_API_H

#include "stdafx.h"

#define LUAAPI

struct LuaApiFunction {
	char* lua_function_name;
	int LUAAPI (*cpp_function)(lua_State*);
};

// Get the list of LUA API functions
struct LuaApiFunction* GetLuaApiFunctionList();

#endif // LUA_API_H
