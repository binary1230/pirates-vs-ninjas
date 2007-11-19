#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "stdafx.h"
#include "gameWorld.h"

class Sprite;

class MapEditor : public GameWorld {
	public:
		virtual int Init(XMLNode);
		virtual void Shutdown();

		virtual void Draw();
		virtual void Update();

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

		XMLNode xObjDefs;

		void Select(Object* obj);
		void SelectPreviousLayer();
		void SelectNextLayer();
		void ToggleOneLayerDisplay();
		void CheckForInput();
};

#endif // MAP_EDITOR_H
