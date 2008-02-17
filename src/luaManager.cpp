#include "stdafx.h"
#include "luaManager.h"
#include "luaApi.h"
#include "assetManager.h"

// For LUA 5.1 and up

DECLARE_SINGLETON(LuaManager);

LuaManager::LuaManager() {m_pkLuaState=NULL;}
LuaManager::~LuaManager() {m_pkLuaState=NULL;}

bool LuaManager::InitBlankLuaState()
{
	if (m_pkLuaState) 
	{
		TRACE("ERROR: A lua script is already loaded! You must call LUA->ReleaseCurrentLuaScript() first!");
		return false;
	}

	if (!( m_pkLuaState = lua_open() ))
	{
		printf("LUA: Error Initializing lua VM!\n");
		return false;
	}

	// init standard lua stuff.
	luaL_openlibs(m_pkLuaState);

	if (!BindLuaCalls()) 
	{
		TRACE("LUA: Failed to bind C++ calls to Lua!\n");
		ReleaseCurrentLuaScript();
		return false;
	}

	return true;
}

bool LuaManager::Init() 
{
	Clear();
	return true;
}

void LuaManager::Clear() 
{
	m_pkLuaState = NULL;
}

/*bool LuaManager::RunVoidFunction1Args(	const char* functionName, 
											const char* arg1,
											bool errorMsgOnFunctionNotFound ) 
{
	const int numArgs = 1;	
	const int numResults = 0;

	lua_getglobal(lua, functionName);

	// not using args right now
	lua_pushnumber(lua, arg1);   // push 1st argument 

	if (lua_pcall(lua, numArgs, numResults, 0) != 0) {
		if (errorMsgOnFunctionNotFound)
			TRACE("LUA: Error running function `%s': %s", 
							functionName, lua_tostring(lua, -1));
		return false;
	}

	// retrieve result 
	// if (!lua_isnumber(lua, -1))
	//   TRACE("function `add_two_things' must return a number");

	// get the return value
	// int result = lua_tonumber(lua, -1);
	// lua_pop(lua, 1);

	return true;

}*/

// Example demonstrating how to call a LUA function from
// C++ with no arguments that returns an int
// Returns true if everything went OK.
bool LuaManager::RunVoidFunctionNoArgs(	const char* functionName, 
										bool errorMsgOnFunctionNotFound) 
{
	if (!m_pkLuaState)
	{
		TRACE("ERROR: Can't call lua function: No file is loaded!");
		return false;
	}

	const int numArgs = 0;	
	const int numResults = 0;

	lua_getglobal(m_pkLuaState, functionName);

	// not using args right now
	//lua_pushnumber(lua, value1);   // push 1st argument 
	//lua_pushnumber(lua, value2);   // push 2nd argument 

	if (lua_pcall(m_pkLuaState, numArgs, numResults, 0) != 0) 
	{
		if (errorMsgOnFunctionNotFound)
			TRACE("LUA: Error running function `%s': %s\n", functionName, lua_tostring(m_pkLuaState, -1));
		return false;
	}

	// retrieve result 
	// if (!lua_isnumber(lua, -1))
	//   TRACE("function `add_two_things' must return a number");

	// get the return value
	// int result = lua_tonumber(lua, -1);
	// lua_pop(lua, 1);

	return true;
}

// run the specified LUA script file
bool LuaManager::LoadLuaScript(const char* _filename) 
{
	if (!InitBlankLuaState())
	{
		ReleaseCurrentLuaScript();
		return false;
	}

	CString filename = ASSETMANAGER->GetPathOf(_filename);

	if (!filename.length()) 
	{
		TRACE("ERROR: Can't open LUA file '%s'\n", _filename);
		ReleaseCurrentLuaScript();
		return false;
	}

	if (luaL_dofile(m_pkLuaState, filename.c_str()) != 0) 
	{
		ReleaseCurrentLuaScript();
		return false;
	}

	return true;
}

void LuaManager::ReleaseCurrentLuaScript() 
{
	if (!m_pkLuaState)
		return;

	lua_close(m_pkLuaState);
	m_pkLuaState = NULL;
}

void LuaManager::Shutdown() 
{
	ReleaseCurrentLuaScript();
}

bool LuaManager::BindLuaCalls() 
{
	assert(m_pkLuaState);
	if (!m_pkLuaState)
	{
		TRACE("ERROR: Can't call lua function: No file is loaded!");
		return false;
	}

	const char* lua_function_name;
	int LUAAPI (*cpp_function)(lua_State*);
	int i = 0;

	struct LuaApiFunction* LuaApiFunctionList = GetLuaApiFunctionList();

	while (1) 
	{
		lua_function_name = LuaApiFunctionList[i].lua_function_name;
		cpp_function = LuaApiFunctionList[i].cpp_function;

		if (!lua_function_name)
			break;

		lua_register(m_pkLuaState, lua_function_name, cpp_function);
		++i;
	}

	return true;
}
