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
#include "globals.h"
#include "gameOptions.h"
#include "gameMode.h"
#include "gameWorld.h"
#include "gameState.h"
#include "objects/objectBackground.h"
#include "objects/objectBounce.h"
#include "objects/objectCollectable.h"
#include "objects/objectController.h"
#include "objects/objectCutBars.h"
#include "objects/objectDoor.h"
#include "objects/objectEnemy.h"
#include "objects/objectFan.h"
#include "objects/objectPlayer.h"
#include "objects/objectSpring.h"
#include "objects/objectStatic.h"
#include "objects/objectTxtOverlay.h"
#include "objectFactory.h"
%}

%module(directors="1") editor
%{
#include "editor.h"
%}

%feature("director") EditorBaseUI;


%include <std_map.i>
%include <std_vector.i>
%include <std_string.i>
%include <attribute.i>

using namespace std;
%template(ObjectVector) std::vector<Object*>;
%template(ObjectLayerVector) std::vector<ObjectLayer*>;

%include "singleton.h"
%include "globals.h"
%include "gameOptions.h"
%include "gameMode.h"
%include "gameWorld.h"
%include "gameState.h"
%include "objectLayer.h"

%typemap(csclassmodifiers) Object "public partial class"
%include "objects/object.h"

%include "objects/objectBackground.h"
%include "objects/objectBounce.h"
%include "objects/objectCollectable.h"
%include "objects/objectController.h"
%include "objects/objectCutBars.h"
%include "objects/objectDoor.h"
%include "objects/objectEnemy.h"
%include "objects/objectFan.h"
%include "objects/objectPlayer.h"
%include "objects/objectSpring.h"
%include "objects/objectStatic.h"
%include "objects/objectTxtOverlay.h"

%include "objectFactory.h"
%include "editor.h"

%typemap(csclassmodifiers) b2Vec2 "public partial class"
struct b2Vec2
{
	float x, y;
};
%attribute(b2Vec2, float, x, x_get, x_set);
%attribute(b2Vec2, float, y, y_get, y_set);

// TODO: implement factory or typemap to create derived types on the C# side