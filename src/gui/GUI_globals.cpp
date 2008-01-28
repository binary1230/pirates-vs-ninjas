#include "stdafx.h"
#include "precompiled.h"
#include "gui/GUI_globals.h"

#ifdef WIN32
#define FLTKCALL __cdecl
#else 
#define FLTKCALL
#endif // WIN32

void FLTKCALL OnLayerSelectionChanged(class Fl_Menu_Button * pkSelection, void *pkUserData)
{
	int x = 3;
}

