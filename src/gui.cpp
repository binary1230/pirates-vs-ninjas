#include "gui.h"

DECLARE_SINGLETON(GuiManager);

GuiManager::GuiManager() {}
GuiManager::~GuiManager() {}

#ifndef WIN32

// TODO: Maybe move to asset manager?
void GuiManager::SetupResourcePaths() {
		using namespace CEGUI;

    // initialise the required dirs for the DefaultResourceProvider
CEGUI::DefaultResourceProvider* rp = 
	static_cast<CEGUI::DefaultResourceProvider*>
  (CEGUI::System::getSingleton().getResourceProvider());

  rp->setResourceGroupDirectory("schemes", "data/cegui/schemes/");
  rp->setResourceGroupDirectory("imagesets", "data/cegui/imagesets/");
  rp->setResourceGroupDirectory("fonts", "data/cegui/fonts/");
  rp->setResourceGroupDirectory("layouts", "data/cegui/layouts/");
  rp->setResourceGroupDirectory("looknfeels", "data/cegui/looknfeel/");
  rp->setResourceGroupDirectory("lua_scripts", "data/cegui/lua_scripts/");
#ifdef CEGUI_WITH_XERCES
  rp->setResourceGroupDirectory("schemas", "data/cegui/XMLRefSchema/");
#endif

	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
#ifdef CEGUI_WITH_XERCES
	CEGUI::XercesParser::setSchemaDefaultResourceGroup("schemas");
#endif
}

#endif // WIN32

bool GuiManager::Init() {
#ifndef WIN32
	renderer = new CEGUI::OpenGLRenderer(0);
	new CEGUI::System(renderer);

	SetupResourcePaths();
	
	CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");

	CEGUI::System::getSingleton().setDefaultFont("Commonwealth-10");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	// Load test root window
	/*
	CEGUI::Window* myRoot = 
		CEGUI::WindowManager::getSingleton().loadWindowLayout("test.layout");

	CEGUI::System::getSingleton().setGUISheet(myRoot);
	*/

	return (renderer != NULL);
#else
	return true;
#endif // WIN32
}

void GuiManager::Draw() {
#ifndef WIN32
	CEGUI::System::getSingleton().renderGUI();
#endif // WIN32
}

void GuiManager::Update() {
}

void GuiManager::Shutdown() {
}
