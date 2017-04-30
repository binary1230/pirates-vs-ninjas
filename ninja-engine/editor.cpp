#include "stdafx.h"
#include "editor.h"
#include "object.h"
#include "objectFactory.h"
#include "gameWorld.h"
#include "input.h"
#include "window.h"
#include "gameState.h"
#include "camera.h"

Object * Editor::CreateObject(const char * objDefName, const char * layerName) {
	XMLNode* xDef = OBJECT_FACTORY->FindObjectDefinition(objDefName);
	assert(xDef);

	string className = OBJECT_FACTORY->GetClassNameFromXML(*xDef);
	Object* obj = Object::CreateObject(className);

	ObjectLayer* layer = WORLD->FindLayer(layerName);
	assert(layer);
	obj->SetLayer(layer);

	obj->SetObjectDefName(objDefName);
	obj->FinishLoading();

	WORLD->AddObject(obj, true);

	_last_object_def_name = objDefName;
	_last_layer_name = layerName;

	if (_ui) 
		_ui->OnObjectsChanged();

	return obj;
}

void Editor::CreateAndSelectObject(const char* objDefName, const char* layerName) {
	_mode = EDITOR_MOVE;
	_should_create_another_copy_after_move = true;
	_should_delete_selection_after_move_done = true;

	Object* obj = CreateObject(objDefName, layerName);

	SelectObject(obj);
	UpdateSelectedObjectPosition();
}

void Editor::CreateAndSelect_UsePreviousLayerAndObject() {
	CreateAndSelectObject(_last_object_def_name.c_str(), _last_layer_name.c_str());
}

// transform mouse input to compensate for scroll speeds on layers
void Editor::MouseToLayerCoords(b2Vec2& layer_coord_out, ObjectLayer* layer) {
	assert(layer);

	b2Vec2 cameraXY;
	WORLD->GetCamera()->GetXY(cameraXY);

	layer_coord_out.x = (INPUT->MouseX() / layer->GetScrollSpeed()) + cameraXY.x;
	layer_coord_out.y = ((WINDOW->Height() - INPUT->MouseY()) / layer->GetScrollSpeed()) + cameraXY.y;
}

void Editor::SnapToGrid(b2Vec2& pos) {
	uint grid_resolution_x = _grid_resolution;
	uint grid_resolution_y = _grid_resolution;

	if (_Selection) {
		grid_resolution_x = _Selection->GetWidth();
		grid_resolution_y = _Selection->GetHeight();

		if (grid_resolution_x < 1)
			grid_resolution_x = _grid_resolution;

		if (grid_resolution_y < 1)
			grid_resolution_y = _grid_resolution;
	}

	pos.x -= int(pos.x) % grid_resolution_x;
	pos.y -= int(pos.y) % grid_resolution_y;
}

void Editor::UpdateSelectedObjectPosition() {
	if (!_Selection)
		return;

	b2Vec2 layer_pos;
	MouseToLayerCoords(layer_pos, _Selection->GetLayer());

	float speed = 1.0f;

	if (INPUT->RealKey(ALLEGRO_KEY_LSHIFT)) {
		speed = 10.0f;
	}

	if (INPUT->RealKey(ALLEGRO_KEY_UP)) {
		offset_change.y += speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_DOWN)) {
		offset_change.y -= speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_RIGHT)) {
		offset_change.x += speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_LEFT)) {
		offset_change.x -= speed;
	}
	if (INPUT->RealKeyOnce(ALLEGRO_KEY_SPACE)) {
		offset_change = b2Vec2(0, 0);
	}

	if (_SnapToGrid) {
		SnapToGrid(layer_pos);
	}

	_Selection->SetXY(layer_pos + offset_change);
}

void Editor::SelectObject(Object* obj) {
	if (_Selection) {
		_Selection->SetDrawBounds(false);
	}

	if (_ui)
		_ui->OnSelectionChanged(obj);

	_Selection = obj;

	if (_Selection) {
		_Selection->SetDrawBounds(true, al_map_rgb(255, 255, 0));
	}
}

void Editor::Draw() {
	if (_text_time_remaining > 0)
		WINDOW->DrawText(5, 5, _tooltip_text);
}

void Editor::CommonUpdate() {
	_ObjectsChanged = false;
	_SelectedObjectChanged = false;

	_pausedChanged = _wasPaused != GAMESTATE->IsPaused();
	if (_pausedChanged) {
		FlashText(!_wasPaused ? "paused" : "unpaused");
	}

	if (_text_time_remaining > 0)
		_text_time_remaining--;
	
	SetDrawBoundingBoxes_AllObjects(false);

	_obj_under_mouse = GetObjectUnderCursor();

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_G)) {
		_SnapToGrid = !_SnapToGrid;
		FlashText(_SnapToGrid ? "grid snap on" : "grid snap off");
	}

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_R)) {
		ResetVolatileLevelState(LEVEL_ITEMS);
	}

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_P)) {
		ResetVolatileLevelState(LEVEL_PLAYERS);
	}
}

void Editor::FlashText(string text) {
	_text_time_remaining = 200;
	_tooltip_text = text;
}

void Editor::NoModeUpdate() {
	if (!GAMESTATE->IsPaused()) {
		SelectObject(NULL);
		return;
	}

	if (_obj_under_mouse && _obj_under_mouse != _Selection)
		_obj_under_mouse->SetDrawBounds(true);

	if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN)) {
		SelectObject(_obj_under_mouse); // can be NULL
	}

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_C)) {
		if (_last_layer_name.length() > 0 && _last_object_def_name.length() > 0) {
			CreateAndSelect_UsePreviousLayerAndObject();
			FlashText("creating objects");
		}
	}

	if (_Selection && INPUT->RealKeyOnce(ALLEGRO_KEY_DELETE)) {
		DeleteCurrentSelection();
	}

	if (_Selection && INPUT->RealKeyOnce(ALLEGRO_KEY_M)) {
		_mode = EDITOR_MOVE;
		_should_delete_selection_after_move_done = false;
		FlashText("move mode");
	}

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_F5)) {
		WORLD->SaveWorldOverCurrentFile();
		FlashText("saved level!");
	}
}

// reset anything "volatile" that shouldn't get saved, like whether we picked up coins, player initial position, etc
void Editor::ResetVolatileLevelState(VolatileStateLevel level) {
	for (Object*& obj : WORLD->_objects) {
		obj->ResetVolatileState(level);
	}

	FlashText("reset'd volatile state");
}

void Editor::UpdateMove() {
	assert(_mode == EDITOR_MOVE);
	assert(_Selection);

	UpdateSelectedObjectPosition();

	if (GAMESTATE->IsPaused()) {
		if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN) || INPUT->RealKeyOnce(ALLEGRO_KEY_ENTER)) {
			_mode = EDITOR_NONE;

			if (_should_create_another_copy_after_move)
				CreateAndSelect_UsePreviousLayerAndObject();
		}
	}

	bool endMode =	INPUT->RealKeyOnce(ALLEGRO_KEY_ESCAPE)	|| 
					INPUT->MouseButtonOnce(MOUSE_RIGHT_BTN) ||
					(_pausedChanged && !GAMESTATE->IsPaused());

	if (endMode) {
		if (_should_delete_selection_after_move_done) {
			DeleteCurrentSelection();
		}
		SelectObject(nullptr);
		_mode = EDITOR_NONE;
		_should_create_another_copy_after_move = false;
		_should_delete_selection_after_move_done = true;
	}
}

Object* Editor::GetObjectUnderCursor() {
	b2Vec2 layer_coords;

	for (Object*& obj : WORLD->_objects) {
		MouseToLayerCoords(layer_coords, obj->GetLayer());

		if (obj->ContainsPoint(layer_coords))
			return obj;
	}

	return NULL;
}

void Editor::SetDrawBoundingBoxes_AllObjects(bool should_draw) {
	for (Object*& obj : WORLD->_objects) {
		if (obj != _Selection)
			obj->SetDrawBounds(false);
	}
}

void Editor::Update() {
	CommonUpdate();

	switch (_mode) {
	case EDITOR_MOVE:
		UpdateMove();
		break;
	case EDITOR_NONE:
		NoModeUpdate();
		break;
	default:
		assert(false && "invalid mode selected");
		break;
	}

	_wasPaused = GAMESTATE->IsPaused();
}

void Editor::DeleteCurrentSelection() {
	if (!_Selection)
		return;

	_ObjectsChanged = true;

	_Selection->SetIsDead(true);
	SelectObject(NULL);

	WORLD->RemoveDeadObjectsIfNeeded();
}

Editor::Editor() {
	_ui = NULL;
	_Selection = NULL;
	_grid_resolution = 30;
	_SnapToGrid = false;
	WORLD->SetPropAllowExiting(false);
	_mode = EDITOR_NONE;
	_wasPaused = GAMESTATE->IsPaused();
	_pausedChanged = false;
	_obj_under_mouse = NULL;

	offset_change = b2Vec2(0.0f, 0.0f);

	_should_create_another_copy_after_move = false;

	_text_time_remaining = 0;
	_tooltip_text = "";

	_ObjectsChanged = false;
	_SelectedObjectChanged = false;

	_should_delete_selection_after_move_done = false;
}

Editor::~Editor() {}

void EditorBaseUI::OnObjectsChanged()
{
}

void EditorBaseUI::OnSelectionChanged(Object * selected_object)
{
}

EditorBaseUI::~EditorBaseUI()
{
}
