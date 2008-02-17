#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "stdafx.h"
#include "gameWorld.h"
#include "mapEditorGui.h"

class Sprite;

enum MapEditMode 
{
	MODE_MAIN,

	MODE_OBJECT_PLACEMENT,
};

class MapEditor : public GameWorld {
	public:
		virtual int Init(XMLNode);
		virtual void Shutdown();

		virtual void Draw();
		virtual void Update();

		// NOTE: THIS FUNCTION DOES NOT RETURN IMMEDIATELY!
		// Instead, it lets LUA call GAMESTATE->Tick() until
		// the GUI has exited.
		virtual void RunMapEditor();

		virtual void LoadMusic(const char* filename);
		virtual int LoadObjectDefsFromXML(XMLNode&);

		void ComputeNewScrolling();

		void SetFlashText(char * format, ... );

		MapEditor();
		virtual ~MapEditor();
	protected:
		Sprite* cursor_sprite;
		Object* selection;

		int m_iCurrentLayer;
		bool m_bDisplayOneLayerOnly;

		CString m_sTxt;
		uint m_uiTxtTicksLeft;

		MapEditMode m_eCurrentMode;

		XMLNode xObjDefs;

		MapEditorGui m_kMapEditorGui;

		void Select(Object* obj);
		void SelectPreviousLayer();
		void SelectNextLayer();
		void ToggleOneLayerDisplay();
		void ModeUpdate();
		void CommonModeUpdateStart();
		void CommonModeUpdateEnd();

		void ModeObjectPlacementUpdate();
		void RePopulateLayerList();
};

#endif // MAP_EDITOR_H
