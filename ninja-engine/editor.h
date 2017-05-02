#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "globals.h"
#include "object.h"

class ObjectLayer;

enum EditorMode {
	EDITOR_NONE,
	EDITOR_MOVE,
	EDITOR_SELECT,
};

// A helper class that will be used as a base class for C# events
class EditorBaseUI {
	public:
		virtual void OnObjectsChanged();
		virtual void OnSelectionChanged();
		virtual void OnSelectedObjectMoved();

		virtual ~EditorBaseUI();
};


//! A helper component for working in map editor mode
class Editor {
	protected:
		uint _grid_resolution;

		string _last_object_def_name;
		string _last_layer_name;

		EditorMode _mode;

		uint _text_time_remaining;
		string _tooltip_text;

		bool _wasPaused;
		bool _pausedChanged;

		bool _should_create_another_copy_after_move;
		bool _should_delete_selection_after_move_done;

		Object* _obj_under_mouse;

		b2Vec2 move_offset;		// offset in move mode when using arrow keys
		b2Vec2 camera_offset;   // offset in no mode when using arrow keys

	public:
		Editor();
		~Editor();

		Object* CreateObject(const char* objDefName, const char* layerName);

		void CreateAndSelectObject(const char * objDefName, const char * layerName);

		void CreateAndSelect_UsePreviousLayerAndObject();

		void MouseToLayerCoords(b2Vec2 & layer_coord_out, ObjectLayer * layer);

		void SnapToGrid(b2Vec2 & pos);

		void UpdateSelectedObjectPosition();

		void SelectObject(Object * obj);

		void Draw();
		void CommonUpdate();
		void CommonAfterUpdate();

		void FlashText(string text);
		void NoModeUpdate();
		void ResetVolatileLevelState(VolatileStateLevel level);
		void UpdateMove();
		Object * GetObjectUnderCursor();
		void SetDrawBoundingBoxes_AllObjects(bool should_draw);
		void Update();

		void DeleteCurrentSelection();
		
		CREATE_PROPERTY(bool, SnapToGrid)
		CREATE_PROPERTY(Object*, Selection)

		CREATE_PROPERTY(EditorBaseUI*, EditorUI)
};

#endif // MAP_EDITOR_H