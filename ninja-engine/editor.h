#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "globals.h"

class Object;

//! A helper component for working in map editor mode
class Editor {
	protected:
		Object* selection;
		uint grid_resolution;

	public:
		Editor();
		~Editor();

		Object* CreateObject(char* objDefName, char* layerName);

		void CreateAndSelectObject(char * objDefName, char * layerName);

		void UpdateSelectedObjectPosition();
		void UnselectCurrentlySelectedObject();

		void Draw();
		void Update();
		
		CREATE_PROPERTY(bool, SnapToGrid)
};

#endif // MAP_EDITOR_H