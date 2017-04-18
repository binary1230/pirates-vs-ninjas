/* 
 * Pirates VS Ninjas - ENGINE interface file for SWIG -> Lua
 * (c) 2008 Dominic Cerquetti
 *
 * This file defines an "interface" that Swig will use
 * to generate C++ code that will be linked into the engine.
 *
 * This interface will then automagically be used to make exported
 * C++ stuff be available inside of any running LUA scripts.
 *
 * todo, #include "../mapEditor.h" in both sections if you want it.
 *
 * */

%begin %{
#include "stdafx.h"
%}

%module engine
%{
#include "gameMode.h"
#include "gameWorld.h"
#include "gameState.h"
#include "objects/objectPlayer.h"
%}

%include <std_map.i>
%include <std_vector.i>
%include <std_string.i>

using namespace std;
%template(ObjectVector) std::vector<Object*>;

%include "singleton.h"
%include "gameMode.h"
%include "gameWorld.h"
%include "gameState.h"
%include "globals.h"
%include "objectLayer.h"
%include "objects/object.h"
%include "objects/objectPlayer.h"