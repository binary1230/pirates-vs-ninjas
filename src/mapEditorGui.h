// All low-level GUI stuff for the map editor
// If possible, someday we'll replace this C++ with lua code that binds to FLTK

#ifndef MAPEDITOR_GUI
#define MAPEDITOR_GUI

class MapEditorGui {
	public:
		bool Init();
		void Shutdown();

		void Update();

		MapEditorGui();
		~MapEditorGui();
		
	protected:
		Fl_Window *m_pkWindow;

		bool UpdateFltk();
};

#endif // MAPEDITOR_GUI
