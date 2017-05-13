#include "stdafx.h"
#include "editor.h"
#include "object.h"
#include "objectFactory.h"
#include "gameWorld.h"
#include "input.h"
#include "window.h"
#include "game.h"
#include "camera.h"

Object * Editor::CreateObject(const char * objDefName, const char * layerName) {
	Object* obj = OBJECT_FACTORY->CreateObject(objDefName);
	
	ObjectLayer* layer = WORLD->FindLayer(layerName);
	assert(layer);
	obj->SetLayer(layer);

	obj->FinishLoading();

	WORLD->AddObject(obj, true);

	_last_object_def_name = objDefName;
	_last_layer_name = layerName;

	if (_EditorUI) 
		_EditorUI->OnObjectsChanged();

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

	b2Vec2 old_pos = _Selection->GetXY();

	b2Vec2 layer_pos;
	MouseToLayerCoords(layer_pos, _Selection->GetLayer());

	float speed = 1.0f;

	if (INPUT->RealKey(ALLEGRO_KEY_LSHIFT)) {
		speed = 10.0f;
	}

	if (INPUT->RealKey(ALLEGRO_KEY_W)) {
		move_offset.y += speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_S)) {
		move_offset.y -= speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_D)) {
		move_offset.x += speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_A)) {
		move_offset.x -= speed;
	}
	if (INPUT->RealKeyOnce(ALLEGRO_KEY_SPACE)) {
		move_offset = b2Vec2(0, 0);
	}

	if (_SnapToGrid) {
		SnapToGrid(layer_pos);
	}

	b2Vec2 new_pos = layer_pos + move_offset;
	_Selection->SetXY(new_pos);

	if (old_pos != new_pos) {
		if (_EditorUI)
			_EditorUI->OnSelectedObjectMoved();
	}
}

void Editor::SelectObject(Object* obj) {
	if (obj == _Selection)
		return;

	if (_Selection) {
		_Selection->SetDrawBounds(false);
	}

	_Selection = obj;

	if (_Selection) {
		_Selection->SetDrawBounds(true, al_map_rgb(255, 255, 0));
	}

	if (_EditorUI)
		_EditorUI->OnSelectionChanged();
}

void Editor::Draw() {
	if (_text_time_remaining > 0)
		WINDOW->DrawText(5, 5, _tooltip_text);
}

void Editor::CommonUpdate() {
	_pausedChanged = _wasPaused != GAME->IsPaused();
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

	float camera_speed = 10.0f;

	if (INPUT->RealKey(ALLEGRO_KEY_LSHIFT)) {
		camera_speed = 100.0f;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_UP)) {
		camera_offset.y += camera_speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_DOWN)) {
		camera_offset.y -= camera_speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_RIGHT)) {
		camera_offset.x += camera_speed;
	}
	if (INPUT->RealKey(ALLEGRO_KEY_LEFT)) {
		camera_offset.x -= camera_speed;
	}
}

void Editor::FlashText(string text) {
	_text_time_remaining = 200;
	_tooltip_text = text;
}

void Editor::NoModeUpdate() {
	if (!GAME->IsPaused()) {
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

	if (_Selection) {
		if (INPUT->RealKeyOnce(ALLEGRO_KEY_DELETE)) {
			DeleteCurrentSelection();
		}

		if (INPUT->RealKeyOnce(ALLEGRO_KEY_M)) {
			_mode = EDITOR_MOVE;
			_should_delete_selection_after_move_done = false;
			FlashText("move mode");
		}

		const int large_step = 50;

		if (INPUT->RealKeyOnce(ALLEGRO_KEY_PGUP)) {
			int step = 1;
			if (INPUT->RealKey(ALLEGRO_KEY_LSHIFT)) {
				step = large_step;
			}

			WORLD->ReorderObject(_Selection, false, step);
			FlashText("re-order up");
			if (_EditorUI)
				_EditorUI->OnObjectsChanged();
		}

		if (INPUT->RealKeyOnce(ALLEGRO_KEY_PGDN)) {
			int step = 1;
			if (INPUT->RealKey(ALLEGRO_KEY_LSHIFT)) {
				step = large_step;
			}

			WORLD->ReorderObject(_Selection, true, step);
			FlashText("re-order down");
			if (_EditorUI)
				_EditorUI->OnObjectsChanged();
		}
	}

	if (INPUT->RealKeyOnce(ALLEGRO_KEY_F5)) {
		FlashText("saving level....");
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

	if (GAME->IsPaused()) {
		if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN) || INPUT->RealKeyOnce(ALLEGRO_KEY_ENTER)) {
			_mode = EDITOR_NONE;

			if (_should_create_another_copy_after_move)
				CreateAndSelect_UsePreviousLayerAndObject();
		}
	}

	bool endMode =	INPUT->RealKeyOnce(ALLEGRO_KEY_ESCAPE)	|| 
					INPUT->MouseButtonOnce(MOUSE_RIGHT_BTN) ||
					(_pausedChanged && !GAME->IsPaused());

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

	CommonAfterUpdate();

	_wasPaused = GAME->IsPaused();
}

void Editor::CommonAfterUpdate() {
	if (!GAME->IsPaused()) {
		camera_offset = b2Vec2(0, 0);
	}
		
	WORLD->GetCamera()->SetCameraOffset(camera_offset);
}

void Editor::DeleteCurrentSelection() {
	if (!_Selection)
		return;

	_Selection->SetIsDead(true);
	SelectObject(NULL);

	WORLD->RemoveDeadObjectsIfNeeded();

	if (_EditorUI)
		_EditorUI->OnObjectsChanged();
}

Editor::Editor() {
	_EditorUI = NULL;
	_Selection = NULL;
	_grid_resolution = 30;
	_SnapToGrid = false;
	WORLD->SetPropAllowExiting(false);
	_mode = EDITOR_NONE;
	_wasPaused = GAME->IsPaused();
	_pausedChanged = false;
	_obj_under_mouse = NULL;

	camera_offset = move_offset = b2Vec2(0.0f, 0.0f);

	_should_create_another_copy_after_move = false;

	_text_time_remaining = 0;
	_tooltip_text = "";

	_should_delete_selection_after_move_done = false;
}

Editor::~Editor() {}

void EditorBaseUI::OnObjectsChanged() {
	assert(0); // should be an abstract class, but for C# reasons, can't be
}

void EditorBaseUI::OnSelectionChanged() {
	assert(0); // should be an abstract class, but for C# reasons, can't be
}

void EditorBaseUI::OnSelectedObjectMoved() {
	assert(0); // should be an abstract class, but for C# reasons, can't be
}

EditorBaseUI::~EditorBaseUI() {
}
