#include "stdafx.h"
#include "gameWorld.h"

#include "luaManager.h"
#include "ai.h"
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

DECLARE_SINGLETON(GameWorld)

#define OBJECT_TEXT	"text_txt"

#define CAMERA_SHAKE false
#define CAMERA_SHAKE_X_MAGNITUDE 15
#define CAMERA_SHAKE_Y_MAGNITUDE 15

int GameWorld::GetCameraX() {
	return m_iCameraX + camera_shake_x;
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

int GameWorld::Init(XMLNode xMode) {
	allow_player_offscreen = false;
	use_scroll_speed = true;
	m_iCameraTotalShakeTime = -1;
	modal_active = NULL;
	m_iLevelWidth = m_iLevelHeight = 0;
	m_iCameraX = m_iCameraY = 0;
	m_pkCameraLookatTarget = NULL;
	m_fCameraScrollSpeed = 1.0f;
	m_bJumpedBackFromADoor = false;
		
	OBJECT_FACTORY->CreateInstance();
	if ( !OBJECT_FACTORY || OBJECT_FACTORY->Init() < 0 ) 
	{
		TRACE("ERROR: InitSystem: failed to init OBJECT_FACTORY!\n");
		return -1;
	}

	EFFECTS->CreateInstance();
	if ( !EFFECTS || !EFFECTS->Init() ) 
	{
		TRACE("ERROR: InitSystem: failed to init EffectsManager!\n");
		return -1;
	}

	m_kObjectsToAdd.clear();
	m_objects.clear();
	m_kLayers.clear();

	EVENTS->CreateInstance();
	if (!EVENTS || !EVENTS->Init()) 
	{
		TRACE("ERROR: InitSystem: failed to init EventsManager!\n");
		return -1;
	}

	PHYSICS->CreateInstance();
	if ( !PHYSICS || !PHYSICS->Init() )
	{
		TRACE("ERROR: InitSystem: failed to init PhysicsManager!\n");
		return -1;
	}

	int iReturn = Load(xMode);

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

	// center the camera on this object
	m_iCameraX = 	int(
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
	m_iCameraX = CAM_MOVE_TO_CENTER(m_iCameraX, ox, ow, sw);
	m_iCameraY = CAM_MOVE_TO_CENTER(m_iCameraY, oy, oh, sh);
	
	// keep it within a certain margin of the sides
	if (ox - m_iCameraX < m_iCameraSideMargins)
		m_iCameraX = ox - m_iCameraSideMargins;
	else if ( (m_iCameraX + sw) - (ox + ow) < m_iCameraSideMargins )
		m_iCameraX = ox + ow + m_iCameraSideMargins - sw;
								
	if (oy - m_iCameraY < m_iCameraSideMargins)
		m_iCameraY = oy - m_iCameraSideMargins;
	else if ( (m_iCameraY + sh) - (oy + oh) < m_iCameraSideMargins )
		m_iCameraY  = oy + oh + m_iCameraSideMargins - sh;
	
	// keep it from getting off screen
	if (m_iCameraX < 0) m_iCameraX = 0;
	if (m_iCameraX > m_iLevelWidth - sw) m_iCameraX = m_iLevelWidth - sw;
	if (m_iCameraY < 0) m_iCameraY = 0;
	if (m_iCameraY > m_iLevelHeight - sh) m_iCameraY = m_iLevelHeight - sh;

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
	ObjectListIter iter;

	if (EVENTS) 
	{
		if (!OPTIONS->MapEditorEnabled())
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
	for (iter = m_objects.begin(); iter != m_objects.end(); iter++) 
	{
		(*iter)->Shutdown();
		delete (*iter);
		(*iter) = NULL;
	}
	m_objects.clear();
	
	// delete all the objects
	for (iter = m_kObjectsToAdd.begin(); iter != m_kObjectsToAdd.end(); iter++) 
	{
		(*iter)->Shutdown();
		delete (*iter);
		(*iter) = NULL;
	}
	m_kObjectsToAdd.clear();
			
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
}

//! Draw all objects in this physics simulation
void GameWorld::Draw() 
{
	// Draw the background gradient first, if we're using it
	if (m_bgColorTop.r != -1.0f) 
	{
		WINDOW->DrawBackgroundGradient(	m_bgColor, m_bgColorTop, 
										m_iCameraY, 
										m_iCameraY + WINDOW->Height(), 
										m_iLevelHeight);
	}

	int i, max = m_kLayers.size();

	for (i = 0; i < max; i++) {
		m_kLayers[i]->Draw();
	}

	// Debug only.
	PHYSICS->Draw();
}

#define CLEAR_SCREEN_STRING "\033[H\033[J\r\n"

void GameWorld::RemoveDeadObjectsIfNeeded() {
	Object* obj;
	
	ObjectListIter iter, erased;
	iter = find_if(m_objects.begin(), m_objects.end(), ObjectIsDead);
	
	while (iter != m_objects.end()) {
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

		erased = iter;
		++iter;
		m_objects.erase(erased);
	
		iter = find_if(iter, m_objects.end(), ObjectIsDead);
	}
}

//! Update all objects
void GameWorld::UpdateObjects() 
{
	AddNewObjectsIfNeeded();
	RemoveDeadObjectsIfNeeded();
	
	// Do the physics simulation + update
	for (ObjectListIter iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
		
		Object* obj = *iter;
		assert(obj != NULL);

		// If there is a 'modal' object, then don't update anything
		// EXCEPT for that object. (usually text boxes/etc) 
		if (!modal_active || obj == modal_active)
		{
			obj->Update();
		}
	}
}

//! Master update 
void GameWorld::Update() {
	DoMainGameUpdate();
}

void GameWorld::DoMainGameUpdate() {

	// If they pressed the 'exit' key (typically ESCAPE)
	// Then end the physics simulation
	if (INPUT->KeyOnce(GAMEKEY_EXIT)) {
    GAMESTATE->SignalGameExit();			// for real
		return;
	}

	for (ObjectListIter iter = m_objects.begin(); iter != m_objects.end(); ++iter) {

		Object* obj = *iter;
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

void GameWorld::LoadMusic(const char* music_file) {
	if (music_file) {
		SOUND->PlayMusic(music_file);
	}
}

//! MASTER LOAD FUNCTION:
//! Load the simulation from data in an XML file
int GameWorld::Load(XMLNode &xMode) {

	is_loading = true;

	m_bJumpedBackFromADoor = false;
	m_objects.clear();
	m_kObjectsToAdd.clear();
	
	if (LoadHeaderFromXML(xMode) == -1)
		return -1;

	if (!PHYSICS->OnWorldInit())
	{
		TRACE("ERROR: InitSystem: failed to init (part 2) PhysicsManager::OnLevelLoaded()!\n");
		return -1;
	}

	if (LoadObjectsFromXML(xMode) == -1) 
	{
		TRACE("ERROR: Failed loading objects from XML\n");
		return -1;
	}

	if (xMode.nChildNode("effects") == 1) {
		m_xEffects = xMode.getChildNode("effects");
		if (!EFFECTS->LoadEffectsFromXML(m_xEffects)) {
			TRACE("ERROR: Can't load Effects XML!\n");
			return -1;
		}
	}
	
	if (!GLOBALS->Value("debug_draw_bounding_boxes", Object::debug_draw_bounding_boxes))
		Object::debug_draw_bounding_boxes = false;

	GLOBALS->Value("camera_side_margins", m_iCameraSideMargins);
	GLOBALS->Value("camera_snap_rate", m_fCameraSnapRate);

	if (xMode.nChildNode("music") == 1) {
		m_szMusicFile = xMode.getChildNode("music").getText();
		LoadMusic(m_szMusicFile);
	}

	exitInfo.useExitInfo = true;

	// special case: if we're coming back from a portal, find it and put the players
	// at that portal's position on the map
	if (lastExitInfo.useExitInfo && lastExitInfo.useLastPortalName) {
		ObjectListIter iter;
		Vector2D portal_pos;
		Object* player;
		bool found = false;

		// find the portal with the specified name 
		for (iter = m_objects.begin(); iter != m_objects.end(); iter++) {
			if ((*iter)->GetProperties().is_door && ((DoorObject*)(*iter))->GetName() == lastExitInfo.lastPortalName) {
				found = true;
				portal_pos = (*iter)->GetXY();
				break;
			}
		}

		if (!found) {
			TRACE("ERROR: Tried to jump to a portal "
											"that doesn't exist named '%s'!\n", 
											lastExitInfo.lastPortalName);
			return -1;
		}

		m_bJumpedBackFromADoor = true;

		// find the player obejcts, set their XY to the portal's XY
		for (iter = m_objects.begin(); iter != m_objects.end(); iter++) {
			if ((*iter)->GetProperties().is_player) {
				player = *iter;
				player->SetXY(portal_pos);
			}
		} 
	}

	// Make sure the camera is centered on 
	// the target right now.
	SnapCamera();

	// Load the LUA file if there is one
	// Don't do this for the map editor, since it needs lua stuff too.
	if (xMode.nChildNode("luaScript") == 1 && !OPTIONS->MapEditorEnabled()) {
		m_szLuaScript = xMode.getChildNode("luaScript").getText();
		LUA->LoadLuaScript(m_szLuaScript);
	}

	is_loading = false;

	if (!OPTIONS->MapEditorEnabled())
		EVENTS->OnLoad();
	
	m_bJumpedBackFromADoor = false;

	CachePlayerObjects();
	
	return 0;	
}

void GameWorld::CachePlayerObjects()
{
	ObjectListIter iter;

	for (iter = m_objects.begin(); iter != m_objects.end(); iter++) 
	{
		assert(*iter != NULL);
		if ((*iter)->GetProperties().is_player ) 
		{
			PlayerObject* player = (PlayerObject*)(*iter);
			m_kCachedPlayers.push_back(player);
		}
	} 
}

// Loads the header info from the Mode XML file
int GameWorld::LoadHeaderFromXML(XMLNode &xMode) {
	XMLNode xInfo = xMode.getChildNode("info");

	TRACE(" Loading Level: '%s'\n", xInfo.getChildNode("description").getText() );

	XMLNode xProps = xMode.getChildNode("properties");
	XMLNode xColor;
	// get width/height/camera xy
	if (!xProps.getChildNode("width").getInt(m_iLevelWidth)) {
		TRACE("-- Invalid width!\n");
		return -1;
	}
	if (!xProps.getChildNode("height").getInt(m_iLevelHeight)) {
		TRACE("-- Invalid height!\n");
		return -1;
	}

	m_bgColor = al_map_rgb_f(0.0f, 0.0f, 0.0f);

	if (xProps.nChildNode("bgcolor") != 1) {
		WINDOW->SetClearColor(0,0,0);
	} else {
		xColor = xProps.getChildNode("bgcolor");
		int r,g,b;
	
		if (!xColor.getChildNode("r").getInt(r) ||
			!xColor.getChildNode("g").getInt(g) ||
			!xColor.getChildNode("b").getInt(b) ||
			r < 0 || g < 0 || b < 0 || r > 255 || b > 255 || g > 255) {
			TRACE("-- Invalid bgcolor specified!\n");
			return -1;
		}

		m_bgColor = al_map_rgb(r,g,b);
		WINDOW->SetClearColor(r,g,b);
	}

	m_bgColorTop = al_map_rgb_f(-1.0f, -1.0f, -1.0f);

	if (xProps.nChildNode("bgcolor_top") == 1) {
		xColor = xProps.getChildNode("bgcolor_top");
		int r,g,b;
	
		if (!xColor.getChildNode("r").getInt(r) ||
			!xColor.getChildNode("g").getInt(g) ||
			!xColor.getChildNode("b").getInt(b) ||
			r < 0 || g < 0 || b < 0 || r > 255 || b > 255 || g > 255) {
			TRACE("-- Invalid bgcolor_top specified!\n");
			return -1;
		}
		m_bgColorTop = al_map_rgb(r, g, b);
	}

	return 0;
}

/* example of how structure of our XML looks:
 * 
 * <mode>
 *
 * 	<objectDefinitions> .. .. .. </objectDefinitions>
 * 
 *	<map>
 * 		<layer>
 * 			<object type="player"> .. .. .. </object>
 * 			<object type="enemy2"> .. .. .. </object>
 * 		</layer>
 * 		<layer> ... </layer>
 * 	</map>
 * 	
 * </mode>
 */

int GameWorld::LoadObjectDefsFromXML(XMLNode &xObjDefs) {
	if (!OBJECT_FACTORY->LoadObjectDefsFromXML(xObjDefs)) 
		return -1;
	else
		return 0;
}

//! Master XML parsing routine for the physics simulation
//! Calls other helpers to deal with different parts of the XML.
int GameWorld::LoadObjectsFromXML(XMLNode &xMode) {	
	int i, max, iterator = 0;  
	XMLNode xMap, xObjDefs, xLayer;

	m_pkCameraLookatTarget = NULL;

	// 1) load all "object definitions" (e.g. [bad guy 1])
	xObjDefs = xMode.getChildNode("objectDefinitions");

	if (LoadObjectDefsFromXML(xObjDefs) != 0)
		return -1;

	// 2) load all the <object>s found in each <layer> in <map>
	xMap = xMode.getChildNode("map");

	max = xMap.nChildNode("layer");

	// Parse each layer
	iterator = 0;
	for (i=0; i < max; i++) {
		xLayer = xMap.getChildNode("layer", &iterator);
		
		ObjectLayer* layer = new ObjectLayer();
		assert(layer != NULL);

		layer->Init();
		m_kLayers.push_back(layer);
		
		if (LoadLayerFromXML(xLayer, layer) == -1) {
			return -1;
		}
	}

	// Finished loading objects, do a few sanity checks
	if (!m_pkCameraLookatTarget) {
		TRACE("ERROR: No <cameraFollow> found, cannot proceed.\n");
		return -1;
	}

	return 0;
}

// Creates an instance of an object on the specified layer 
int GameWorld::CreateObjectFromXML(XMLNode &xObject, ObjectLayer* const layer) {

		// get the object definition name
		CString objDefName = xObject.getAttribute("objectDef");

		// try to find that object definition
		XMLNode* xObjectDef = OBJECT_FACTORY->FindObjectDefinition(objDefName);

		if (!xObjectDef) {
			TRACE("ERROR: Unable to find object definition of type '%s'\n", 
											objDefName);
			return -1;
		}

		// create the object from the objectDefinition
		if (LoadObjectFromXML(*xObjectDef, xObject, layer) == -1) {
			TRACE("ERROR: Failed trying to load object of type '%s'\n", 
											objDefName);
			return -1;
		}

		return 0;
}

//! Parse XML info from a <layer> block
int GameWorld::LoadLayerFromXML(XMLNode &xLayer, ObjectLayer* const layer) {

	int i, iterator, max;
	XMLNode xObject;
	CString objDefName;

	// 1) How much do we scroll this layer by?
	float scroll_speed;
	if ( !xLayer.getAttributeFloat("scroll_speed", scroll_speed) ) {
		TRACE(" -- no scroll_speed specified.\n");
		return -1;
	}

	layer->SetScrollSpeed(scroll_speed);
	layer->SetName(xLayer.getAttribute("name"));
	
	// 2) NEW: special case.  Because I, Dom, am LAZY as HELL, I have
	// added a <REPEAT> tag which allows us to create, say, 50
	// objects while only having to declare just one (combine this
	// with random positions, and you have an interesting formula for
	// random level generation!)
	//
	// Note: this whole "repeat" thing is almost a hack and won't 
	// be needed once we have an actual map editor in place.
	int times_to_repeat, j;
	XMLNode xRepeater;
	max = xLayer.nChildNode("repeat");

	for (i=iterator=0; i < max; i++) {

		xRepeater = xLayer.getChildNode("repeat", &iterator);

		if (!xRepeater.getAttributeInt("times", times_to_repeat)) {
			TRACE("-- Invalid # repeat times!\n");
			return -1;
		}

		repeater_current_x = 0;
		repeater_current_y = 0;

		if (xRepeater.nChildNode("starting_x") == 1) {
			if (!xRepeater.getChildNode("starting_x").getInt(repeater_current_x)) {
				TRACE("ERROR: Invalid starting_x specified in <repeat>\n");
				return -1;
			}
		} 
		
		if (xRepeater.nChildNode("starting_y") == 1) {
			if (!xRepeater.getChildNode("starting_y").getInt(repeater_current_y)) {
				TRACE("ERROR: Invalid starting_y specified in <repeat>\n");
				return -1;
			}
		}

		xObject = xRepeater.getChildNode("object");

		// Repeat the creation of this object the specified # of times.
		for (j=0; j < times_to_repeat; j++) {
			if (CreateObjectFromXML(xObject, layer) == -1)
				return -1;
		}	
	}

	max = xLayer.nChildNode("object");

	for (i=iterator=0; i < max; i++) {
		xObject = xLayer.getChildNode("object", &iterator);

		if (CreateObjectFromXML(xObject, layer) == -1)
			return -1;
	}

	return 0;
}

// Do the REAL work of loading an object from XML
int GameWorld::LoadObjectFromXML(XMLNode &xObjectDef,
								 XMLNode &xObject,
								 ObjectLayer* const layer) {

	int x,y;

	// Really create the instance of this object, it is BORN here:
	Object* obj = OBJECT_FACTORY->CreateObjectFromXML(xObjectDef, xObject);

	if (!obj)
		return -1;

	obj->SetLayer(layer);

	// if we have a <cameraFollow>, then we follow this object
	if (xObject.nChildNode("cameraFollow") == 1) {
		if (!m_pkCameraLookatTarget) {
			m_pkCameraLookatTarget = obj;
		} else {
			TRACE("ERROR: multiple camera targets in map\n");
			return -1;
		}
	}

	// SPECIAL debug flag.  IF it is set, the object MAY print debug message
	if (xObject.nChildNode("debug") == 1) {
		TRACE("-- Enabling debug mode.\n");
		obj->SetDebugFlag(true);
	}

	if (xObject.nChildNode("position") == 1) {
		XMLNode xPos = xObject.getChildNode("position");
		CString type = xPos.getAttribute("type");
		// Figure out the position type.
		// Currently 3 types exist:
		// 1) "fixed" - regular XY position, nothing fancy
		//
		// 2) "random" - pick random numbers in a range for the XY position
		// 							 (This would be useful for e.g. making 50 randomly
		// 							 placed flowers in a level)
		//
		// 3) "offset" - specify the distance from which to place this object
		//               from the last one
		//               (This would be useful for e.g. making 20 fenceposts 
		//               exactly 10 pixels from each other)
		//               
		// Note that 2) and 3) are only really useful inside an XML <repeat>
		// tag.  You can use them to position tons of objects with only one 
		// line of XML.  2) and 3) won't ever be used unless someone is
		// hand-coding the XML.  Once the map editor is done, only 1) will
		// be useful.

		if (type == CString("fixed")) {

			if (!xPos.getChildNode("x").getInt(x)) {
				TRACE("-- Invalid X coordinate specified (or did you want <x> instead of <x_offset> ?\n");
				return -1;	
			}

			if (!xPos.getChildNode("y").getInt(y)) {
				TRACE("-- Invalid Y coordinate specified (or did you want <y> instead of <y_offset> ?\n");
				return -1;
			}
				
		} else if (type == CString("random")) {

			int xmin, ymin, xmax, ymax;

			if (!xPos.getChildNode("xmin").getInt(xmin)) {
				TRACE("-- Invalid xmin!\n");
				return -1;
			}

			if (!xPos.getChildNode("ymin").getInt(ymin)) {
				TRACE("-- Invalid ymin!\n");
				return -1;
			}

			if (!xPos.getChildNode("xmax").getInt(xmax)) {
				TRACE("-- Invalid xmax!\n");
				return -1;
			}

			if (!xPos.getChildNode("ymax").getInt(ymax)) {
				TRACE("-- Invalid ymax!\n");
				return -1;
			}

			x = Rand(xmin, xmax);
			y = Rand(ymin, ymax);	

		} else if (type == CString("offset")) {

			int _offset_x, _offset_y;
			if (!xPos.getChildNode("x_offset").getInt(_offset_x)) {
				TRACE("-- Invalid X!\n");
				return -1;	
			}
			if (!xPos.getChildNode("y_offset").getInt(_offset_y)) {
				TRACE("-- Invalid Y!\n");
				return -1;
			}

			x = repeater_current_x;
			y = repeater_current_y;

			repeater_current_x += _offset_x;
			repeater_current_y += _offset_y;
				
		} else {
			TRACE("Unknown object position type: %s\n", type);
			return -1;
		}
				
		// if <alignTop> is present, we align this sprite with ITs 
		// bottom coordinates. (e.g. saying 0 puts the player on the floor)
		if (xPos.nChildNode("alignTop")>0) {
			y -= obj->GetHeight();
		}
			
		// if <alignRight> is present, we take the X coordinate from the
		// right side instead of the left.
		if (xPos.nChildNode("alignRight")>0) {
			x -= obj->GetWidth();
		}

		// if <alignScreenRight> is present, we align this sprite
		// to the SCREEN's right (useful only for overlays)
		if (xPos.nChildNode("alignScreenRight")>0) {
			x = WINDOW->Width() - obj->GetWidth() - x;
		}

		// if <alignScreenBottom> is present, we align this sprite
		// to the SCREEN's bottom (useful only for overlays)
		if (xPos.nChildNode("alignScreenBottom")>0) {
			y = WINDOW->Height() - obj->GetHeight() - y;
		}

		// One last position calculation:
		// We need to undo the offset of the background here
		// So users don't have to compensate in their data files
		if (layer->GetScrollSpeed() > 0.01f) {
			x = int( float(x) / layer->GetScrollSpeed() );
			y = int( float(y) / layer->GetScrollSpeed() );
		}

		// flipping
		if (xPos.nChildNode("flipx")>0) {
			obj->SetFlipX(true);
		}

		if (xPos.nChildNode("flipy")>0) {
			obj->SetFlipY(true);
		}
		
		// this can freak out physics
		assert(obj->GetX() <= WORLD->GetWidth() + 10);
		assert(obj->GetY() <= WORLD->GetHeight() + 10);

		obj->SetXY(x,y);

		// check for velocity - <velx>, <vely>, and <vel_rotate>
		/*
		if (xPos.nChildNode("velx")>0) {
			float velx;
			if (!xPos.getChildNode("velx").getFloat(velx)) {
				TRACE("-- Invalid velx!\n");
				return -1;
			}
			obj->SetVelX(velx);
		}

		if (xPos.nChildNode("vely")>0) {
			float vely;
			if (!xPos.getChildNode("vely").getFloat(vely)) {
				TRACE("-- Invalid vely!\n");
				return -1;
			}
			obj->SetVelY(vely);
		}
		*/

		if (xPos.nChildNode("vel_rotate")>0) {
			float vel_rotate;
			if (!xPos.getChildNode("vel_rotate").getFloat(vel_rotate)) {
				TRACE("-- Invalid vel_rotate!\n");
				return -1;
			}
			obj->SetUseRotation(true);
			obj->SetVelRotate(vel_rotate);
		}

	}	// end of <position> stuff
			
	if (xObject.nChildNode("inputController") == 1) {
		int controller_num;
		if (!xObject.getChildNode("inputController").getInt(controller_num)) {
			TRACE("-- Invalid controller number!\n");
			return -1;
		}
		obj->SetControllerNum(controller_num);
	}

	if (xObject.nChildNode("alpha") == 1) {
		int alpha;
		if (!xObject.getChildNode("alpha").getInt(alpha) || alpha > 255) {
			TRACE("-- Invalid alpha!\n");
			return -1;
		}
		obj->SetAlpha(alpha);
	}

	if (xObject.nChildNode("fadeout") == 1) {
		int fadeout_time;
		if (!xObject.getChildNode("fadeout").getInt(fadeout_time)) {
			TRACE("-- Invalid fadeout time!\n");
			return -1;
		}

		if (fadeout_time > 0)
			obj->FadeOut(fadeout_time);
	}

	// Everything loaded OK, now we add it to the simulation
	AddObject(obj, true);

	return 0;
}

ObjectLayer* GameWorld::FindLayer(const char* name) {
	for (uint i = 0; i < m_kLayers.size(); ++i) {
		if (stricmp(m_kLayers[i]->GetName(),name) == 0)
			return m_kLayers[i];
	}

	return NULL;
}

void GameWorld::AddObject(Object* obj, bool addImmediately) {
	assert(obj != NULL);
	assert(obj->GetLayer() != NULL);

	if (addImmediately) {
		DoAddObject(obj);
	} else {
		m_kObjectsToAdd.push_back(obj);
	}
}

void GameWorld::DoAddObject(Object* obj) {
	m_objects.push_front(obj);
	obj->InitPhysics();
	obj->GetLayer()->AddObject(obj);
}

#ifndef AI_TRAINING
int GameWorld::GetAiFitnessScore() {return 0;};
#else
int GameWorld::GetAiFitnessScore() {
	ObjectListIter iter;
	
	for (iter = m_objects.begin(); iter != m_objects.end(); iter++) {
		assert(*iter != NULL);
		if (	(*iter)->GetProperties().is_player ) {
			PlayerObject* player = (PlayerObject*)(*iter);
			return player->GetNumRings();
		}
	} 

	return 0;
}
#endif // AI_TRAINING

GameWorld::GameWorld() {
	is_loading = false;
	m_bIsCameraShaking = CAMERA_SHAKE; //temp, usually false
	m_iCameraTotalShakeTime = -1;
}

GameWorld::~GameWorld() {}

void GameWorld::AddNewObjectsIfNeeded()
{
	// Add any New Objects
	for (ObjectListIter iter = m_kObjectsToAdd.begin(); iter != m_kObjectsToAdd.end(); ++iter) {
		Object* obj = *iter;
		assert(obj != NULL);
		AddObject(obj, true);
	}

	m_kObjectsToAdd.clear();
}