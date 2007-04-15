#ifndef _GUI_H
#define _GUI_H

#include "stdafx.h"

#ifndef WIN32
class CEGUI::OpenGLRenderer;
#endif // WIN32

class GuiManager {
	DECLARE_SINGLETON_CLASS(GuiManager);

	protected:
		virtual ~GuiManager();
	
#ifndef WIN32
		CEGUI::OpenGLRenderer* renderer;
#endif // WIN32
	
		void SetupResourcePaths();

	public:
		bool Init();
		void Shutdown();

		void Draw();
		void Update();
};

#define GUI (GuiManager::GetInstance())

#endif // _GUI_H
