#include "stdafx.h"
#include "gameWorld.h"

#include "luaManager.h"
#include "assetManager.h"
#include "globals.h"
#include "objectFactory.h"
#include "object.h"
#include "objectIDs.h"
#include "objectTxtOverlay.h"
#include "input.h"
#include "gameState.h"
#include "objectLayer.h"
#include "window.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "objectPlayer.h"
#include "effectsManager.h"
#include "objectDoor.h"
#include "globalDefines.h"
#include "eventManager.h"
#include "physics.h"
#include "editor.h"

DECLARE_SINGLETON(GameWorld)

#define OBJECT_TEXT	"text_txt"

#define CAMERA_SHAKE false
#define CAMERA_SHAKE_X_MAGNITUDE 15
#define CAMERA_SHAKE_Y_MAGNITUDE 15

int GameWorld::GetCameraX() {
	return _iCameraX + camera_shake_x;
}

int GameWorld::GetCameraY() {
	return m_iCameraY + camera_shake_y;
}

void GameWorld::ShowText(	const char* txt, 
							const char* avatar_filename, 
							bool modal_active) {
	
	ObjectText* obj = (ObjectText*)OBJECT_FACTORY->CreateObject(OBJECT_TEXT);

	if (!obj) {
		TRACE("ERROR: Failed to create Txt object in ShowText()\n");
		return;
	}

	obj->SetText(txt);
	obj->SetModalActive(modal_active);

	if (avatar_filename && strlen(avatar_filename) > 0)
		obj->SetAvatarFilename(avatar_filename);

	AddObject(obj);
}

void GameWorld::Clear() {
	is_loading = false;
	map_editor = NULL;
	_AllowExiting = true;
	
	_objects.clear();
	_layers.clear();
	m_included_effect_xml_files.clear();
	m_included_objectdef_xml_files.clear();

	m_bIsCameraShaking = CAMERA_SHAKE;
	_bgColorTop = _bgColor = al_map_rgb_f(0.0f, 0.0f, 0.0f);
	allow_player_offscreen = false;
	use_scroll_speed = true;
	m_iCameraTotalShakeTime = -1;
	modal_active = NULL;
	_levelWidth = _levelHeight = 0;
	_iCameraX = m_iCameraY = 0;
	m_pkCameraLookatTarget = NULL;
	m_fCameraScrollSpeed = 1.0f;
	m_bJumpedBackFromADoor = false;
	_objectsToAdd.clear();
	m_iCameraShakeTime = 0;
	m_iCameraSideMargins = 40;
	m_fCameraSnapRate = 3.0f;
	camera_shake_x = 0;
	camera_shake_y = 0;
}

int GameWorld::Init(XMLNode /*unused*/) {
	if (OPTIONS->GetMapEditorEnabled())
		map_editor = new Editor();

	OBJECT_FACTORY->CreateInstance();
	if ( !OBJECT_FACTORY || OBJECT_FACTORY->Init() < 0 ) 
	{
		TRACE("ERROR: InitSystems: failed to init OBJECT_FACTORY!\n");
		return -1;
	}

	EFFECTS->CreateInstance();
	if ( !EFFECTS || !EFFECTS->Init() ) 
	{
		TRACE("ERROR: InitSystems: failed to init EffectsManager!\n");
		return -1;
	}

	EVENTS->CreateInstance();
	if (!EVENTS || !EVENTS->Init()) 
	{
		TRACE("ERROR: InitSystems: failed to init EventsManager!\n");
		return -1;
	}

	PHYSICS->CreateInstance();
	if ( !PHYSICS || !PHYSICS->Init() )
	{
		TRACE("ERROR: InitSystems: failed to init PhysicsManager!\n");
		return -1;
	}

	int iReturn = Load();

	if (iReturn != 0)
		return iReturn;

	return iReturn;
}

//! Transforms view coordinates into absolute screen coordinates
//! e.g. flip the Y axis mostly.
void GameWorld::TransformViewToScreen(	int &x, int &y ) {
	y = WINDOW->Height() - y;
}

//! Transforms an object's coordinates from its world coordinates
//! Into "view" coordinates (e.g. x < screen.width, y < screen.height)
void GameWorld::TransformWorldToView(int &x, int &y) {
	x = int((x - GetCameraX() ) * m_fCameraScrollSpeed);
	y = int((y - GetCameraY() ) * m_fCameraScrollSpeed);
	// y = y - GetCameraY();
}

//! Weighted average nums for the 'floaty' camera
//! Increase CAM_WEIGHT_CAM to make the camera 'snap' quicker
#define CAM_WEIGHT_POS 1.0f // DON'T CHANGE.

//! Function which moves the camera according to a weight, shown above
//! Uses a weighted average of the object coordinates and the new camera coords
#define CAM_MOVE_TO_CENTER(cam, o, o_size, s_size) 									\
	int( float( 																											\
			(((o + o_size / 2.0f) - (s_size / 2.0f)) * CAM_WEIGHT_POS) + 	\
			((cam) * m_fCameraSnapRate) 																			\
		) / (m_fCameraSnapRate + CAM_WEIGHT_POS) )

// Snap the camera to its target object
// Useful when switching targets
void GameWorld::SnapCamera() {

	assert(m_pkCameraLookatTarget);

	// center the camera on this object
	_iCameraX = 	int(
							(
								 float(m_pkCameraLookatTarget->GetX()) + 
								(float(m_pkCameraLookatTarget->GetWidth()) / 2.0f)
							) -	(
								(float(WINDOW->Height()) / 2.0f)
							));

	m_iCameraY = 	int(
							(
						 		 float(m_pkCameraLookatTarget->GetY()) + 
								(float(m_pkCameraLookatTarget->GetHeight()) / 2.0f)
							) -	(
								(float(WINDOW->Width()) / 2.0f)
							));

	// now do the nice computations
	ComputeNewCamera();
}

void GameWorld::SetCameraShake(bool state, int fadeout_time) {
	m_iCameraShakeTime = 0;
	m_iCameraTotalShakeTime = fadeout_time;
	m_bIsCameraShaking = state;

	if (!m_bIsCameraShaking)
		m_iCameraTotalShakeTime = -1;
}

// Oy, vey, this is a bit more complex than it needs to be?
void GameWorld::ComputeNewCamera() {
	
	assert(m_pkCameraLookatTarget != NULL);
				
	int ox = m_pkCameraLookatTarget->GetX();
	int ow = m_pkCameraLookatTarget->GetWidth();
	int sw = WINDOW->Width();
	
	int oy = m_pkCameraLookatTarget->GetY();
	int oh = m_pkCameraLookatTarget->GetHeight();
	int sh = WINDOW->Height();
	
	// compute the next interpolated position
	_iCameraX = CAM_MOVE_TO_CENTER(_iCameraX, ox, ow, sw);
	m_iCameraY = CAM_MOVE_TO_CENTER(m_iCameraY, oy, oh, sh);
	
	// keep it within a certain margin of the sides
	if (ox - _iCameraX < m_iCameraSideMargins)
		_iCameraX = ox - m_iCameraSideMargins;
	else if ( (_iCameraX + sw) - (ox + ow) < m_iCameraSideMargins )
		_iCameraX = ox + ow + m_iCameraSideMargins - sw;
								
	if (oy - m_iCameraY < m_iCameraSideMargins)
		m_iCameraY = oy - m_iCameraSideMargins;
	else if ( (m_iCameraY + sh) - (oy + oh) < m_iCameraSideMargins )
		m_iCameraY  = oy + oh + m_iCameraSideMargins - sh;
	
	// keep it from getting off screen
	if (_iCameraX < 0) _iCameraX = 0;
	if (_iCameraX > _levelWidth - sw) _iCameraX = _levelWidth - sw;
	if (m_iCameraY < 0) m_iCameraY = 0;
	if (m_iCameraY > _levelHeight - sh) m_iCameraY = _levelHeight - sh;

	// do the camera shake
	if (!m_bIsCameraShaking) {
		camera_shake_x = 0;
		camera_shake_y = 0;
	} else {
		float multiplier = 1.0f;

		if (m_iCameraTotalShakeTime != -1) {
			if (m_iCameraShakeTime >= m_iCameraTotalShakeTime) {
				m_bIsCameraShaking = false;
				multiplier = 0.0f;
			} else {
				++m_iCameraShakeTime;
				multiplier = 1.0f - float(m_iCameraShakeTime) / float(m_iCameraTotalShakeTime);
			}
		}

		camera_shake_x = Rand(0, float(CAMERA_SHAKE_X_MAGNITUDE) * multiplier);
		camera_shake_y = Rand(0, float(CAMERA_SHAKE_Y_MAGNITUDE) * multiplier);
	}
}

void GameWorld::Shutdown() 
{
	if (EVENTS) 
	{
		if (!map_editor)
			EVENTS->OnUnLoad();

		EVENTS->Shutdown();
		EVENTS->FreeInstance();
	}

	if (PHYSICS)
	{
		PHYSICS->Shutdown();
		PHYSICS->FreeInstance();
	}

	// delete all the objects
	for (Object*& obj : _objects)
	{
		obj->Shutdown();
		delete obj;
		obj = NULL;
	}
	_objects.clear();
	for (Object*& obj : _objectsToAdd)
	{
		obj->Shutdown();
		delete obj;
		obj = NULL;
	}
	_objectsToAdd.clear();
			
	// delete the object factory
	if (OBJECT_FACTORY) 
	{
		OBJECT_FACTORY->Shutdown();
		OBJECT_FACTORY->FreeInstance();
	}

	// delete the effects manager
	if (EFFECTS) 
	{
		EFFECTS->Shutdown();
		EFFECTS->FreeInstance();
	}

	LUA->ReleaseCurrentLuaScript();

	if (map_editor) {
		delete map_editor;
	}
}

//! Draw all objects in this physics simulation
void GameWorld::Draw() 
{
	// Draw the background gradient first, if we're using it
	if (_bgColorTop.r != -1.0f) 
	{
		WINDOW->DrawBackgroundGradient(	_bgColor, _bgColorTop, 
										m_iCameraY, 
										m_iCameraY + WINDOW->Height(), 
										_levelHeight);
	}

	int i, max = _layers.size();

	for (i = 0; i < max; i++) {
		_layers[i]->Draw();
	}

	PHYSICS->Draw();

	if (map_editor)
		map_editor->Draw();
}

void GameWorld::RemoveDeadObjectsIfNeeded() {
	Object* obj;
	
	auto iter = find_if(_objects.begin(), _objects.end(), [](const Object* obj) { return obj->IsDead(); });
	
	while (iter != _objects.end()) {
		obj = *iter;
		assert(obj != NULL);

		if (modal_active == obj)
			modal_active = NULL;

		if (obj == m_pkCameraLookatTarget) {
			assert(0 && "ERROR: CheckIsDead(): Deleted camera object!!");
			m_pkCameraLookatTarget = NULL;
		}

		obj->Shutdown();
		delete obj;
		*iter = NULL;

		auto erased = iter;
		++iter;
		_objects.erase(erased);
	
		iter = find_if(iter, _objects.end(), [](const Object* obj) { return obj->IsDead(); });
	}
}

void GameWorld::UpdateObjects()  
{
	AddNewObjectsIfNeeded();
	RemoveDeadObjectsIfNeeded();
	
	for (Object*& obj : _objects) {		
		assert(obj != NULL);

		// If there is a 'modal' object, then don't update anything
		// EXCEPT for that object. (usually text boxes/etc) 
		if (!modal_active || obj == modal_active)
		{
			obj->Update();
		}
	}
}

void GameWorld::Update() {
	if (_AllowExiting && INPUT->KeyOnce(GAMEKEY_EXIT)) {
		GAMESTATE->SignalGameExit();
		return;
	}

	if (!GAMESTATE->IsPaused()) {
		DoMainGameUpdate();
	}

	if (map_editor)
		map_editor->Update();
}

void GameWorld::DoMainGameUpdate() {
	for (Object*& obj : _objects) {
		assert(obj != NULL);

		// If there is a 'modal' object, then don't update anything
		// EXCEPT for that object. (usually text boxes/etc) 
		if (!modal_active || obj == modal_active)
			obj->ResetForNextFrame();
	}

	if (!modal_active)
		PHYSICS->Update();

	UpdateObjects();
	ComputeNewCamera();						// Calc where to put the camera now
}

void GameWorld::SaveWorldOverCurrentFile() 
{
	SaveWorld(exitInfo.lastModeName);
}

void GameWorld::SaveWorld(string filename)
{
	std::ofstream ofs(filename);
	assert(ofs.good());
	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(this);
}

GameWorld* GameWorld::CreateWorld(string mode_filename = "") {
	GameWorld* unserialized_world = NULL;
	std::ifstream ifs(mode_filename);
	boost::archive::xml_iarchive ia(ifs);
	ia >> BOOST_SERIALIZATION_NVP(unserialized_world);

	WORLD->SetInstance(unserialized_world);

	return unserialized_world;
}

void GameWorld::LoadMusic(const char* music_file) {
	if (music_file && !map_editor) {
		SOUND->PlayMusic(music_file);
	}
}

int GameWorld::Load() {
	is_loading = true;
	m_bJumpedBackFromADoor = false;
	_objectsToAdd.clear();

	WINDOW->SetClearColor(_bgColor.r, _bgColor.g, _bgColor.b);

	if (!PHYSICS->OnWorldInit())
	{
		TRACE("ERROR: InitSystems: failed to init (part 2) PhysicsManager::OnLevelLoaded()!\n");
		return -1;
	}

	if (!LoadObjectDefsFromXML())
		return -1;

	if (!FinishLoadingObjects())
		return -1;

	for (int i = 0; i < m_included_effect_xml_files.size(); ++i)
	{
		EFFECTS->LoadEffectsFromIncludedXml(m_included_effect_xml_files[i]);
	}

	if (!GLOBALS->Value("debug_draw_bounding_boxes", Object::debug_draw_bounding_boxes))
		Object::debug_draw_bounding_boxes = false;

	GLOBALS->Value("camera_side_margins", m_iCameraSideMargins);
	GLOBALS->Value("camera_snap_rate", m_fCameraSnapRate);

	if (_musicFile.length() > 0) {
		LoadMusic(_musicFile.c_str());
	}

	if (!InitJumpBackFromDoor()) {
		return -1;
	}

	if (!m_pkCameraLookatTarget) {
		TRACE("ERROR: No <cameraFollow> found, cannot proceed.\n");
		return -1;
	}

	// Make sure the camera is centered on 
	// the target right now.
	SnapCamera();

	// Load the LUA file if there is one
	// Don't do this for the map editor, since it needs lua stuff too.
	if (!map_editor && _luaScript.length() > 0) {
		LUA->LoadLuaScript(_luaScript.c_str());
	}

	is_loading = false;

	if (!map_editor)
		EVENTS->OnLoad();
	
	m_bJumpedBackFromADoor = false;

	CachePlayerObjects();
	
	return 0;	
}

bool GameWorld::FinishLoadingObjects()
{
	for (Object*& obj : _objects) {
		if (!obj->FinishLoading())
			return false;
	}	

	return true;
}

bool GameWorld::InitJumpBackFromDoor()
{
	exitInfo.useExitInfo = true;

	// special case: if we're coming back from a portal, find it and put the players
	// at that portal's position on the map
	if (lastExitInfo.useExitInfo && lastExitInfo.useLastPortalName) {
		b2Vec2 portal_pos;
		bool found = false;

		// find the portal with the specified name 
		for (Object*& obj : _objects) {
			if (ObjectDoor* door = dynamic_cast<ObjectDoor*>(obj)) {
				if (door->GetName() == lastExitInfo.lastPortalName) {
					found = true;
					portal_pos = obj->GetXY();
					break;
				}
			}
		}

		if (!found) {
			TRACE("ERROR: Tried to jump to a portal that doesn't exist named '%s'!\n", lastExitInfo.lastPortalName);
			return false;
		}

		m_bJumpedBackFromADoor = true;

		// find the player obejcts, set their XY to the portal's XY
		for (Object*& obj : _objects) {
			if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {
				player->SetXY(portal_pos);
			}
		}
	}
	return true;
}

void GameWorld::CachePlayerObjects() {
	for (Object*& obj : _objects) {
		assert(obj != NULL);

		if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {
			m_kCachedPlayers.push_back(player);
		}
	}
}


bool GameWorld::LoadObjectDefsFromXML() {
	for (string& objectDefXmlFile : m_included_objectdef_xml_files) {
		if (!OBJECT_FACTORY->LoadObjectDefsFromIncludeXML(objectDefXmlFile)) {
			return false;
		}
	}

	return true;
}

ObjectLayer* GameWorld::FindLayer(const char* name) {
	for (uint i = 0; i < _layers.size(); ++i) {
		if (stricmp(_layers[i]->GetName(),name) == 0)
			return _layers[i];
	}

	return NULL;
}

void GameWorld::AddObject(Object* obj, bool addImmediately) {
	assert(obj != NULL);
	assert(obj->GetLayer() != NULL);

	if (addImmediately) {
		_objects.push_front(obj);
		obj->InitPhysics();
		obj->GetLayer()->AddObject(obj);
	} else {
		_objectsToAdd.push_back(obj);
	}
}

GameWorld::GameWorld() {
	Clear();
}

GameWorld::~GameWorld() {}

void GameWorld::AddNewObjectsIfNeeded()
{
	// Add any New Objects
	for (Object*& obj : _objectsToAdd) {
		assert(obj != NULL);
		AddObject(obj, true);
	}

	_objectsToAdd.clear();
}

BOOST_SERIALIZATION_ASSUME_ABSTRACT(GameWorld)
BOOST_CLASS_EXPORT_GUID(GameWorld, "GameWorld")

// increment this anytime we change the archive contents
BOOST_CLASS_VERSION(GameWorld, 1)