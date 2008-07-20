/* 
 * Pirates VS Ninjas - ENGINE interface file for SWIG -> Lua
 * (c) 2008 Dominic Cerquetti
 *
 * This file defines an "interface" that Swig will use
 * to generate C++ code that will be linked into the engine.
 *
 * This interface will then automagically be used to make exported
 * C++ stuff be available inside of any running LUA scripts.
 * */

%module engine
%{
#include "../stdafx.h"
#include "../gameMode.h"
#include "../gameWorld.h"
#include "../mapEditor.h"
#include "../gameState.h"
%}

%include "../singleton.h"
%include "../gameMode.h"
%include "../gameWorld.h"
%include "../mapEditor.h"
%include "../gameState.h"
