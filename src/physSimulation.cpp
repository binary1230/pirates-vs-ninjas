#include "physSimulation.h"

#include "ai.h"
#include "assetManager.h"
#include "globals.h"
#include "objectFactory.h"
#include "object.h"
#include "objectIDs.h"
#include "forceFactory.h"
#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"
#include "input.h"
#include "gameState.h"
#include "objectLayer.h"
#include "window.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "objectPlayer.h"

int PhysSimulation::Init(XMLNode xMode) {
	modal_active = NULL;
	music = NULL;
	width = height = 0;
	camera_x = camera_y = 0;
	camera_follow = NULL;
	camera_scroll_speed = 1.0f;
	
	OBJECT_FACTORY->CreateInstance();
	if ( !OBJECT_FACTORY || OBJECT_FACTORY->Init() < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init OBJECT_FACTORY!\n");
		return -1;
	}

	OBJECT_FACTORY->SetPhysSimulation(this);

	forceFactory = new ForceFactory();
	if ( !forceFactory || forceFactory->Init() < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init forceFactory!\n");
		return -1;
	}

	objectAddList.clear();
	objects.clear();
	forces.clear();
	layers.clear();
	
	return Load(xMode);
}

//! Transforms view coordinates into absolute screen coordinates
//! e.g. flip the Y axis mostly.
void PhysSimulation::TransformViewToScreen(	int &x, int &y ) {
	y = WINDOW->Height() - y;
}

//! Transforms an object's coordinates from its world coordinates
//! Into "view" coordinates (e.g. x < screen.width, y < screen.height)
void PhysSimulation::TransformWorldToView(int &x, int &y) {
	x = int((x - GetCameraX() ) * camera_scroll_speed);
	y = int((y - GetCameraY() ) * camera_scroll_speed);
	// y = y - GetCameraY();
}

//! Camera paramater: Absolute amount of pixels that 
//! the camera_follow object will be bounded at
#define CAM_ABS_THRESHOLD 40

//! Weighted average nums for the 'floaty' camera
//! Increase CAM_WEIGHT_CAM to make the camera 'snap' quicker
#define CAM_WEIGHT_POS 1.0f // DON'T CHANGE.
#define CAM_WEIGHT_CAM 3.0f	// change this one for faster camera 'slides'

//! Function which moves the camera according to a weight, shown above
//! Uses a weighted average of the object coordinates and the new camera coords
#define CAM_MOVE_TO_CENTER(cam, o, o_size, s_size) 									\
	int( float( 																											\
			(((o + o_size / 2.0f) - (s_size / 2.0f)) * CAM_WEIGHT_POS) + 	\
			((cam) * CAM_WEIGHT_CAM) 																			\
		) / (CAM_WEIGHT_CAM + CAM_WEIGHT_POS) )

// Oy, vey, this is a bit more complex than it needs to be?
void PhysSimulation::ComputeNewCamera() {
	
	assert(camera_follow);
				
	int ox = camera_follow->GetX();
	int ow = camera_follow->GetWidth();
	int sw = WINDOW->Width();
	
	int oy = camera_follow->GetY();
	int oh = camera_follow->GetHeight();
	int sh = WINDOW->Height();
	
	camera_x = CAM_MOVE_TO_CENTER(camera_x, ox, ow, sw);
	camera_y = CAM_MOVE_TO_CENTER(camera_y, oy, oh, sh);
	
	if (ox - camera_x < CAM_ABS_THRESHOLD)
		camera_x = ox - CAM_ABS_THRESHOLD;
	else if ( (camera_x + sw) - (ox + ow) < CAM_ABS_THRESHOLD )
		camera_x = ox + ow + CAM_ABS_THRESHOLD - sw;
								
	if (oy - camera_y < CAM_ABS_THRESHOLD)
		camera_y = oy - CAM_ABS_THRESHOLD;
	else if ( (camera_y + sh) - (oy + oh) < CAM_ABS_THRESHOLD )
		camera_y  = oy + oh + CAM_ABS_THRESHOLD - sh;
	
	if (camera_x < 0) camera_x = 0;
	if (camera_x > width - sw) camera_x = width - sw;
	if (camera_y < 0) camera_y = 0;
	if (camera_y > height - sh) camera_y = height - sh;
}

void PhysSimulation::Shutdown() {
	ObjectListIter iter;
	int max, i;

	// delete all the objects
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		(*iter)->Shutdown();
		delete (*iter);
		(*iter) = NULL;
	}
	objects.clear();
	
	// delete all the objects
	for (iter = objectAddList.begin(); iter != objectAddList.end(); iter++) {
		(*iter)->Shutdown();
		delete (*iter);
		(*iter) = NULL;
	}
	objectAddList.clear();

	// delete all the forces
	max = forces.size();
	for (i = 0; i < max; i++) {
		forces[i]->Shutdown();
		delete forces[i];
		forces[i] = NULL;
	}
	forces.clear();
			
	// delete the object factory
	if (OBJECT_FACTORY) {
		OBJECT_FACTORY->Shutdown();
		OBJECT_FACTORY->FreeInstance();
	}

	// delete the force factory
	if (forceFactory) {
		forceFactory->Shutdown();
		delete forceFactory;
		forceFactory = NULL;
	}
}

//! Draw all objects in this physics simulation
void PhysSimulation::Draw() {
	int i, max = layers.size();

	for (i = 0; i < max; i++) {
		layers[i]->Draw();
	}
}

#define CLEAR_SCREEN_STRING "\033[H\033[J\r\n"

// Returns true if we deleted the object
// Returns false if the Object is still valid
bool PhysSimulation::CleanupObject(ObjectListIter &iter) {
	ObjectListIter erased;
	Object* obj = *iter;

	assert(obj != NULL);

	// if it's dead _after_ the update, clean it up
	if (obj->IsDead()) {
		
		if (modal_active == obj)
			modal_active = NULL;

		// Delete ths object.
		obj->Shutdown();
		delete obj; 

		// Delete the object's place in the list
		/**iter = NULL;
		erased = iter;
		iter++;
		objects.erase(erased);
		iter--;
		
		erased = iter;
		iter++;*/

		//objects.erase(iter);
		//++iter;

		// SANITY CHECK
		if (obj == camera_follow) {
			assert(0 && "ERROR: CheckIsDead(): Deleted camera object!!");
			camera_follow = NULL;
		}
		
		return true;
	}

	return false;
}

//! Solve for next frame
void PhysSimulation::Solve(Object *obj) {
	
	assert(obj != NULL);
	int j, max = forces.size();

	// apply each force to the object
	for (j = 0; j < max; j++) {
		obj->ApplyForce(forces[j]);
	}
}

void PhysSimulation::GetCollideableObjects(ObjectList &objs) {
	ObjectListIter iter;
	objs.clear();

	// optimization: only allow collisions on certain layers?
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		if (Object::CanCollide(*iter))
			objs.push_back(*iter);
	}
}


// TODO: probably a BIG source of CPU here.
// probably need to optimize, but PROFILE to find out.
void PhysSimulation::CheckForCollisions(	ObjectList &collideableObjects, 
																					Object* obj) {
	assert(obj != NULL);

	Object* target;
	ObjectListIter iter;

	// Don't bother to check if we can't be collided with.
	if (!Object::CanCollide(obj))
		return;
	
	// Loop over all collectable objects, see if we collide with any
	for (	iter = collideableObjects.begin(); 
				iter != collideableObjects.end(); 
				iter++) {

		target = *iter;

		// Skip ourselves
		if (target == obj)
			continue;

		if (target->IsColliding(obj)) {
			target->Collide(obj);
			obj->Collide(target);
		}
	}
}

bool ObjectDeadCriteria(const Object* obj) {
	return obj->IsDead();
}

// A generic template to delete a collection of pointers
/*void objects_purge(ObjectListIter begin, ObjectListIter end)
{
}*/

void PhysSimulation::DoCleaning() {
	ObjectListIter last;
	
	// Remove these elements
	last = partition(objects.begin(), objects.end(), ObjectDeadCriteria);

	// delete the pointers
	for (ObjectListIter i = last; i != objects.end(); ++i) {
		if (modal_active == (*i))
			modal_active = NULL;

		(*i)->Shutdown();
		delete (*i);
	}

	// remove the items from the list
	objects.erase(last, objects.end());
}

//! Update all objects
void PhysSimulation::UpdateObjects() {
	ObjectList collideableObjects;
	ObjectListIter iter;
	Object* obj;

	static int ucount = 0;
	ucount++;
	
	fprintf(stderr, "starting update %i\n", ucount);

	// Add any New Objects
	for (iter = objectAddList.begin(); iter != objectAddList.end(); iter++) {
		obj = *iter;
		assert(obj != NULL);
		DoAddObject(obj);
	}
	
	DoCleaning();

	// Cleanup Dead Objects
	/*for (iter = last; iter != objects.end(); iter++) {
		assert(*iter != NULL);
		CleanupObject(iter);
	}*/
	
	fprintf(stderr, "mid update %i\n", ucount);

	// Get collideable objects
	GetCollideableObjects(collideableObjects);
	
	fprintf(stderr, "mid2 update %i\n", ucount);

	// Do the physics simulation + update
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		obj = *iter;
		assert(obj != NULL);

		// If there is a 'modal' object, then don't update anything
		// EXCEPT that. (usually text boxes/etc)
		if (!modal_active) {
			obj->ResetForNextFrame();				// oldpos = current_pos
			Solve(obj);											// Applies forces
			obj->MoveToNewPosition();
			CheckForCollisions(collideableObjects, obj);		// newpos = oldpos
		}
 
		if (!modal_active || obj == modal_active)
			obj->Update();
	}
	
	fprintf(stderr, "end update %i\n", ucount);
}

//! Master update for the Physics simulation
void PhysSimulation::Update() {

	// If they pressed the 'exit' key (typically ESCAPE)
	// Then end the physics simulation
	if (INPUT->KeyOnce(GAMEKEY_EXIT)) {
		// use one or the other, SignalGameExit() is "right"
		// SignalEndCurrentMode() goes to the next level.

    // GAMESTATE->SignalGameExit();			// for real
    GAMESTATE->SignalEndCurrentMode(); 	// for debugging
		return;
	}

	// If something modal is active, do NOT do physics stuff, 
	// just update this ONE object.
	//if (modal_active) {
		//modal_active->Update();
		//fprintf(stderr, "modal done!\n");
		//return;
	//}
		
	UpdateObjects();
	ComputeNewCamera();						// Calc where to put the camera now
}

//! MASTER LOAD FUNCTION:
//! Load the simulation from data in an XML file
int PhysSimulation::Load(XMLNode &xMode) {			

	objects.clear();
	objectAddList.clear();
	forces.clear();
	if (LoadHeaderFromXML(xMode) == -1 ||
			LoadObjectsFromXML(xMode) == -1 ||
			LoadForcesFromXML(xMode) == -1 )
		return -1;

	camera_x = camera_follow->GetX();
	camera_y = camera_follow->GetY();

	if (xMode.nChildNode("music") == 1) {
		const char* music_file = xMode.getChildNode("music").getText();
		SOUND->LoadMusic(music_file);
		SOUND->PlayMusic();
	}
	
	return 0;	
}

// Loads the header info from the Mode XML file
int PhysSimulation::LoadHeaderFromXML(XMLNode &xMode) {
	XMLNode xInfo = xMode.getChildNode("info");

	fprintf(stderr, " Loading Level: '%s'\n", xInfo.getChildNode("description").getText() );

	XMLNode xProps = xMode.getChildNode("properties");
	XMLNode xColor;
	int clear_color;

	// get width/height/camera xy
	if (!xProps.getChildNode("width").getInt(width)) {
		fprintf(stderr, "-- Invalid width!\n");
		return -1;
	}
	if (!xProps.getChildNode("height").getInt(height)) {
		fprintf(stderr, "-- Invalid height!\n");
		return -1;
	}
	if (!xProps.getChildNode("camera_x").getInt(camera_x)) {
		fprintf(stderr, "-- Invalid camera_x!\n");
		return -1;
	}
	if (!xProps.getChildNode("camera_y").getInt(camera_y)) {
		fprintf(stderr, "-- Invalid camera_y!\n");
		return -1;
	}

	if (xProps.nChildNode("bgcolor") == 1) {
		xColor = xProps.getChildNode("bgcolor");
		int r,g,b;
	
		if (	!xColor.getChildNode("r").getInt(r) ||
					!xColor.getChildNode("g").getInt(g) ||
					!xColor.getChildNode("b").getInt(b) ||
					r < 0 || g < 0 || b < 0 || r > 255 || b > 255 | g > 255) {
					fprintf(stderr, "-- Invalid bgcolor specified!\n");
					return -1;
		}

		clear_color = makecol(r,g,b);
	} else {
		clear_color = 0;
	}

	// XXX make clear_color it a class memeber
	WINDOW->SetClearColor(clear_color);

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
	
//! Master XML parsing routine for the physics simulation
//! Calls other helpers to deal with different parts of the XML.
int PhysSimulation::LoadObjectsFromXML(XMLNode &xMode) {	
  int i, max, iterator = 0;  
	XMLNode xMap, xObjDefs, xLayer;

	camera_follow = NULL;

	// 1) load all "object definitions" (e.g. [bad guy 1])
	xObjDefs = xMode.getChildNode("objectDefinitions");
	OBJECT_FACTORY->LoadObjectDefsFromXML(xObjDefs);

	// 2) load all the <object>s found in each <layer> in <map>
	xMap = xMode.getChildNode("map");

	max = xMap.nChildNode("layer");

	// Parse each layer
	iterator = 0;
  for (i=0; i < max; i++) {
		xLayer = xMap.getChildNode("layer", &iterator);
		
		ObjectLayer* layer = new ObjectLayer();
		layer->Init(this);
		layers.push_back(layer);
		
		if (LoadLayerFromXML(xLayer, layer) == -1) {
			return -1;
		}
	}

	// Finished loading objects, do a few sanity checks
	if (!camera_follow) {
		fprintf(stderr, "ERROR: No <cameraFollow> found, cannot proceed.\n");
		return -1;
	}

	return 0;
}

// Creates an instance of an object on the specified layer 
int PhysSimulation::CreateObjectFromXML(XMLNode &xObject, ObjectLayer* layer) {

		// get the object definition name
		CString objDefName = xObject.getAttribute("objectDef");

		// try to find that object definition
		XMLNode* xObjectDef = OBJECT_FACTORY->FindObjectDefinition(objDefName);

		if (!xObjectDef) {
			fprintf(stderr, "ERROR: Unable to find object definition of type '%s'\n", 
											objDefName.c_str());
			return -1;
		}

		// create the object from the objectDefinition
		if (LoadObjectFromXML(*xObjectDef, xObject, layer) == -1) {
			fprintf(stderr, "ERROR: Failed trying to load object of type '%s'\n", 
											objDefName.c_str());
			return -1;
		}

		return 0;
}

//! Parse XML info from a <layer> block
int PhysSimulation::LoadLayerFromXML(XMLNode &xLayer, ObjectLayer* layer) {

	int i, iterator, max;
	XMLNode xObject;
	CString objDefName;

	// 1) How much do we scroll this layer by?
	float scroll_speed;
  if ( !xLayer.getAttributeFloat("scroll_speed", scroll_speed) ) {
		fprintf(stderr, " -- no scroll_speed specified.\n");
		return -1;
	}

	layer->SetScrollSpeed(scroll_speed);
	
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
			fprintf(stderr, "-- Invalid # repeat times!\n");
			return -1;
		}

		repeater_current_x = 0;
		repeater_current_y = 0;

		if (xRepeater.nChildNode("starting_x") == 1) {
			if (!xRepeater.getChildNode("starting_x").getInt(repeater_current_x)) {
				fprintf(stderr, "ERROR: Invalid starting_x specified in <repeat>\n");
				return -1;
			}
		} 
		
		if (xRepeater.nChildNode("starting_y") == 1) {
			if (!xRepeater.getChildNode("starting_y").getInt(repeater_current_y)) {
				fprintf(stderr, "ERROR: Invalid starting_y specified in <repeat>\n");
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
int PhysSimulation::LoadObjectFromXML(
								XMLNode &xObjectDef,
								XMLNode &xObject,
								ObjectLayer* layer) {

	int x,y;

	// Really create the instance of this object, it is BORN here:
	Object* obj = OBJECT_FACTORY->CreateObjectFromXML(xObjectDef, xObject);

	if (!obj) {
		return -1;
	} else {

		obj->SetLayer(layer);

		// if we have a <cameraFollow>, then we follow this object
		if (xObject.nChildNode("cameraFollow") == 1) {
			if (!camera_follow) {
				camera_follow = obj;
			} else {
				fprintf(stderr, "ERROR: multiple camera targets in map\n");
				return -1;
			}
		}

		// SPECIAL debug flag.  IF it is set, the object MAY print debug message
		if (xObject.nChildNode("debug") == 1) {
			fprintf(stderr, "-- Enabling debug mode.\n");
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
					fprintf(stderr, "-- Invalid X!\n");
					return -1;	
				}
				if (!xPos.getChildNode("y").getInt(y)) {
					fprintf(stderr, "-- Invalid Y!\n");
					return -1;
				}
				
			} else if (type == CString("random")) {

				int xmin, ymin, xmax, ymax;

				if (!xPos.getChildNode("xmin").getInt(xmin)) {
					fprintf(stderr, "-- Invalid xmin!\n");
					return -1;
				}

				if (!xPos.getChildNode("ymin").getInt(ymin)) {
					fprintf(stderr, "-- Invalid ymin!\n");
					return -1;
				}

				if (!xPos.getChildNode("xmax").getInt(xmax)) {
					fprintf(stderr, "-- Invalid xmax!\n");
					return -1;
				}

				if (!xPos.getChildNode("ymax").getInt(ymax)) {
					fprintf(stderr, "-- Invalid ymax!\n");
					return -1;
				}

				x = Rand(xmin, xmax);
				y = Rand(ymin, ymax);	
			} else if (type == CString("offset")) {

				int _offset_x, _offset_y;
				if (!xPos.getChildNode("x_offset").getInt(_offset_x)) {
					fprintf(stderr, "-- Invalid X!\n");
					return -1;	
				}
				if (!xPos.getChildNode("y_offset").getInt(_offset_y)) {
					fprintf(stderr, "-- Invalid Y!\n");
					return -1;
				}

				x = repeater_current_x;
				y = repeater_current_y;
	
				repeater_current_x += _offset_x;
				repeater_current_y += _offset_y;
				
			} else {
				fprintf(stderr, "Unknown object position type: %s\n", type.c_str());
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
				obj->SetFlipX(false);
			}
		
			obj->SetXY(x,y);

			// check for velocity - <velx> and <vely>
			if (xPos.nChildNode("velx")>0) {
				float velx;
				if (!xPos.getChildNode("velx").getFloat(velx)) {
					fprintf(stderr, "-- Invalid velx!\n");
					return -1;
				}
				obj->SetVelX(velx);
			}
			if (xPos.nChildNode("vely")>0) {
				float vely;
				if (!xPos.getChildNode("vely").getFloat(vely)) {
					fprintf(stderr, "-- Invalid vely!\n");
					return -1;
				}
				obj->SetVelY(vely);
			}
		}
			
		if (xObject.nChildNode("inputController") == 1) {
			int controller_num;
			if (!xObject.getChildNode("inputController").getInt(controller_num)) {
				fprintf(stderr, "-- Invalid controller number!\n");
				return -1;
			}
			obj->SetControllerNum(controller_num);
		}

		if (xObject.nChildNode("alpha") == 1) {
			int alpha;
			if (!xObject.getChildNode("alpha").getInt(alpha) || alpha > 255) {
				fprintf(stderr, "-- Invalid alpha!\n");
				return -1;
			}
			obj->SetAlpha(alpha);
		}

		if (xObject.nChildNode("fadeout") == 1) {
			int fadeout_time;
			if (!xObject.getChildNode("fadeout").getInt(fadeout_time)) {
				fprintf(stderr, "-- Invalid fadeout time!\n");
				return -1;
			}
			if (fadeout_time > 0)
				obj->FadeOut(fadeout_time);
		}

		// Everything loaded OK, now we add it to the simulation
		DoAddObject(obj);
	}

	return 0;
}

void PhysSimulation::AddObject(	Object* obj, bool addImmediately) {
	assert(obj != NULL);
	assert(obj->GetLayer() != NULL);

	if (addImmediately) {
		DoAddObject(obj);
	} else {
		objectAddList.push_back(obj);
	}
}

void PhysSimulation::DoAddObject(Object* obj) {
	assert(obj != NULL);
	assert(obj->GetLayer() != NULL);
	objects.push_front(obj);
	obj->GetLayer()->AddObject(obj);
}

// loads the forces from the XML file
int PhysSimulation::LoadForcesFromXML(XMLNode &xMode) {
	
	// XXX need to actually load from XML instead of hardcoding...
	Force* new_force = NULL;

	if ( (new_force = forceFactory->CreateForce(FORCE_GRAVITY)) )
		forces.push_back(new_force);
	else
		return -1;
	
	if ( (new_force = forceFactory->CreateForce(FORCE_INPUT1)) )
		forces.push_back(new_force);
	else
		return -1;

	if ( (new_force = forceFactory->CreateForce(FORCE_INPUT2)) )
		forces.push_back(new_force);
	else
		return -1;
	
	return 0;
}

#ifndef AI_TRAINING
int PhysSimulation::GetAiFitnessScore() {return 0;};
#else
int PhysSimulation::GetAiFitnessScore() {
	ObjectListIter iter;
	
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		assert(*iter != NULL);
		if (	(*iter)->GetProperties().is_player ) {
			PlayerObject* player = (PlayerObject*)(*iter);
			return player->GetNumRings();
		}
	} 

	return 0;
}
#endif // AI_TRAINING

PhysSimulation::PhysSimulation() {}
PhysSimulation::~PhysSimulation() {}
