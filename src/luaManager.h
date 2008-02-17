#ifndef LUA_H
#define LUA_H

/* Deals with all lua-related setup and init */

#include "stdafx.h"

class LuaManager;

class LuaManager {
	DECLARE_SINGLETON_CLASS(LuaManager)
	
	public:
		bool Init();
		void Shutdown();

		// Clear out any lua state
		void Clear();

		// Run a .lua file with our engine calls exposed
		bool LoadLuaScript(const char* filename);
		void ReleaseCurrentLuaScript();

		bool RunVoidFunctionNoArgs(	const char* functionName, 
																bool errorMsgOnFunctionNotFound = true);
		
		bool RunVoidFunction1Args(	const char* functionName, 
																const char* arg1,
																bool errorMsgOnFunctionNotFound = true);

	protected:
		lua_State* m_pkLuaState; 

		// Bind our C/C++ API to Lua, done on init
		bool BindLuaCalls();

		virtual ~LuaManager();

		bool InitBlankLuaState();
};

#define LUA (LuaManager::GetInstance())

#endif // LUA_H
