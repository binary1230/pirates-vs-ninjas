#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

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

		void UpdateSelectedObjectPosition();
		void UnselectCurrentlySelectedObject();

		void Draw();
		void Update();

		bool snap_to_grid;
};

#endif // MAP_EDITOR_H