#include <stdio.h>
extern "C"
{
 #include <lua.h>
 #include <lualib.h>
 #include <lauxlib.h>
}

#define LUAAPI // just define it

int LUAAPI lua_printTheThing( lua_State* luaVM) {
	printf("%s", lua_tostring(luaVM, -1) );
	lua_pushnumber( luaVM, 0 );
	return 1;
}
 
int main(int argc, char* argv[ ])
{
   lua_State* luaVM = lua_open();
 
   if (NULL == luaVM)
   {
      printf("Error Initializing lua\n");
      return -1;
   }
 
   // initialize lua standard library functions

   lua_baselibopen(luaVM);
   lua_iolibopen(luaVM);
   lua_strlibopen(luaVM);
   lua_mathlibopen(luaVM);

	 // init our stuff. 

	 lua_register( luaVM, "printTheThing", lua_printTheThing );

   // Do stuff with lua code.
    
   lua_dofile(luaVM, "./dom.lub");

	 // done.
	 
   lua_close(luaVM);   

   return 0;
}
