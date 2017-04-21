#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

class Object;

//! A helper component for working in map editor mode
class Editor {
	public:
		Editor();
		~Editor();

		Object* CreateObject(char* objDefName, char* layerName);

		void UpdateSelectedObjectPosition();
		void UnselectCurrentlySelectedObject();

		void Draw();
		void Update();

	private:
		Object* selection;
};

#endif // MAP_EDITOR_H