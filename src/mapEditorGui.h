// All low-level GUI stuff for the map editor
// Mainly a wrapper for the lua scripts which do the real work

#ifndef MAPEDITOR_GUI
#define MAPEDITOR_GUI

class MapEditorGui {
	public:
		bool Init();
		void Shutdown();

		void Start();
		void Update();

		MapEditorGui();
		~MapEditorGui();
};

#endif // MAPEDITOR_GUI
