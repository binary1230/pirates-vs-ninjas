#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "gameWorld.h"
#include "mapEditorGui.h"

class Sprite;

enum MapEditMode 
{
	MODE_MAIN,
	MODE_OBJECT_PLACEMENT,
	MODE_HELP,
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

		// A resolution of 1 means disable grid
		void SetGridResolution(int iResolution) { m_iGridResolution = iResolution; }

		MapEditor();
		virtual ~MapEditor();
	protected:
		Sprite* cursor_sprite;

		int m_iCurrentLayer;
		bool m_bDisplayOneLayerOnly;

		CString m_sTxt;
		uint m_uiTxtTicksLeft;

		MapEditMode m_eCurrentMode;

		XMLNode xObjDefs;

		MapEditorGui m_kMapEditorGui;

		int m_iCurrentObjectDefinitionIndex;
		Object* m_pkSelectedObject;
		bool m_bIsCurrentSelectionNewObject;

		int m_iGridResolution;

		void SelectPreviousLayer();
		void SelectNextLayer();
		void ToggleOneLayerDisplay();
		void ModeUpdate();
		void CommonModeUpdateStart();
		void CommonModeUpdateEnd();

		void UpdateSelectedObjectLayer();

		void UpdateCurrentObjectDefinitionIfNeeded();

		void AddNewObjectToWorld( int iObjectDefinitionIndexToAdd );

		void UpdateSelectedObjectPosition();

		void UnselectCurrentlySelectedObject();

		void RemoveCurrentlySelectedObject();
		void RePopulateLayerList();

		void DrawCommonModeStuff();
		void DrawHelpMode();

		void ModeObjectPlacementUpdate();
		void ModeMainUpdate();
		void ModeHelpUpdate();
};

#endif // MAP_EDITOR_H
