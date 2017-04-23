#include "stdafx.h"
#include "editor.h"
#include "object.h"
#include "objectFactory.h"
#include "gameWorld.h"
#include "input.h"
#include "window.h"

Object * Editor::CreateObject(char * objDefName, char * layerName)
{
	XMLNode* xDef = OBJECT_FACTORY->FindObjectDefinition(objDefName);
	assert(xDef);

	string className = OBJECT_FACTORY->GetClassNameFromXML(*xDef);
	Object* obj = Object::CreateObject(className);
	
	obj->create_physics_body = false;

	ObjectLayer* layer = WORLD->FindLayer(layerName);
	assert(layer);
	obj->SetLayer(layer);

	selection = obj;
	UpdateSelectedObjectPosition();

	obj->SetObjectDefName(objDefName);
	obj->FinishLoading();

	WORLD->AddObject(obj, true);

	return obj;
}

void Editor::UpdateSelectedObjectPosition()
{
	if (!selection)
		return;

	selection->SetDrawBounds(true);

	assert(selection->GetLayer());

	// transform mouse input to compensate for scroll speeds on layers
	int x = (int)(INPUT->MouseX() / selection->GetLayer()->GetScrollSpeed()) + WORLD->m_iCameraX;
	int y = (int)((WINDOW->Height() - INPUT->MouseY()) / selection->GetLayer()->GetScrollSpeed()) + WORLD->m_iCameraY;

	if (snap_to_grid)
	{
		x -= x % grid_resolution;
		y -= y % grid_resolution;
	}

	selection->SetXY(x, y);
}

void Editor::UnselectCurrentlySelectedObject()
{
	if (!selection)
		return;

	selection->SetDrawBounds(false);

	selection->create_physics_body = true;
	selection->InitPhysics();

	// just remove our reference to it, it's already inserted into the world.
	selection = NULL;
}

void Editor::Draw()
{

}

void Editor::Update()
{
	UpdateSelectedObjectPosition();

	// insert it permanently into world if they clicked
	if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN))
		UnselectCurrentlySelectedObject();
}

Editor::Editor()
{
	selection = NULL;
	grid_resolution = 30;
	snap_to_grid = false;
}

Editor::~Editor()
{
}