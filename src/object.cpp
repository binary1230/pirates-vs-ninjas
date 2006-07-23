#include "object.h"
#include "gameState.h"
#include "window.h"
#include "force.h"
#include "globals.h"
#include "gameBase.h"
#include "animation.h"
#include "animations.h"
#include "physSimulation.h"
#include "xmlParser.h"
#include "gameOptions.h"
#include "StdString.h"
#include "gameSound.h"
#include "objectLayer.h"

void Object::PlaySound(CString name) {
	GetGameState()->GetSound()->PlaySound(name);
}

// GetWidth() and GetHeight() need rethinking - 
// they use the first frame's width and height
int Object::GetWidth() { 
	// return currentAnimation->GetWidth();
	
	if (animations.size() > 0 && animations[0])
		return animations[0]->GetWidth();
	else
		return 0;
}

int Object::GetHeight() {	
	// return currentAnimation->GetHeight();
	if (animations.size() > 0 && animations[0])
		return animations[0]->GetHeight();
	else
		return 0;
}

//! Cache some commonly used stuff
void Object::SetupCachedVariables() {
	simulation = GetGameState()->GetPhysSimulation();
	level_width  = simulation->GetWidth();
	level_height = simulation->GetHeight();
}

bool Object::BaseInit() {
	SetupCachedVariables();
	ClearProperties(properties);
	is_dead = false;
	return true;
}

void Object::Draw() {
	assert(simulation != NULL);
	DrawAtOffset(0,0);
}

//! Ultimately we need the actual, on-screen coordinates of where
//! to draw the sprite.  To get to those from the object's "world" coordinates
//! as computed by the physics engine, we need to take into account the 
//! position of the camera, and we need to flip the Y axis.  These
//! things are handled by the simulation->TransformXXX() methods.
//
//! This function populates x,y (reference params) with the 
//! correctly transformed coordinates.
void Object::Transform(int &x, int &y, int offset_x, int offset_y) {
	x = (int)pos.GetX() + offset_x;
	y = (int)pos.GetY() + offset_y;

	// take into account the camera now.
	if (!properties.is_overlay)
		simulation->TransformWorldToView(x, y);
	
	// compute absolute x,y coordinates on the screen
	y = y + GetHeight();
	simulation->TransformViewToScreen(x, y);
}

// Same as Transform(), just for rectangles only.
void Object::TransformRect(Rect &r) {

	int x1, x2, y1, y2, w, h;
	r.Fix();
	x1 = (int)r.getx1();	
	y1 = (int)r.gety1();	
	x2 = (int)r.getx2();
	y2 = (int)r.gety2();
	w = x2 - x1;
	h = y2 - y1;

	// take into account the camera now.
	if (!properties.is_overlay) {
		simulation->TransformWorldToView(x1, y1);
		simulation->TransformWorldToView(x2, y2);
	}
	
	// compute absolute x,y coordinates on the screen
	simulation->TransformViewToScreen(x1, y1);
	simulation->TransformViewToScreen(x2, y2);

	r.setx1(x1); 	r.sety1(y1);
	r.setx2(x2); 	r.sety2(y2);
	// r.setx2(x1 + w); 	r.sety2(y1 + h);
}

//! This function does the real dirty work of drawing.
void Object::DrawAtOffset(int offset_x, int offset_y, Sprite* sprite_to_draw) {	
	int x, y;
	Transform(x, y, offset_x, offset_y);
				
	// draw for real
	if (!sprite_to_draw)
		sprite_to_draw = currentSprite;
	
	if (sprite_to_draw)
		GetGameState()->GetWindow()->
		DrawSprite(sprite_to_draw, x, y, flip_x, flip_y);

	#define DEBUG_DRAW_BOUNDING_BOXES 0

	if (!DEBUG_DRAW_BOUNDING_BOXES)
		return;

	Rect bbox_t;
	Rect projRect_t = projRect;
	Rect bbox_t_old = bbox;

	// get current bounding box
	bbox_t.setx1(pos.GetX());
	bbox_t.sety1(pos.GetY());
	bbox_t.setx2(pos.GetX() + GetWidth());
	bbox_t.sety2(pos.GetY() + GetHeight());

	// draw projection rectangle, blue
	TransformRect(bbox_t);
	TransformRect(bbox_t_old);
	TransformRect(projRect_t);

	if (	properties.is_player || 
				properties.is_solid || 
				properties.is_collectable) {

		// draw old bounding rectangle, dark pink
		GetGameState()->GetWindow()->
		DrawRect(bbox_t_old, makecol(127,0,127));
	
		// draw current bounding rectangle, pink
		GetGameState()->GetWindow()->
		DrawRect(bbox_t, makecol(255,0,255));

		// draw projection rectangle, blue
		GetGameState()->GetWindow()->
		DrawRect(projRect_t, makecol(0, 0, 255));
	}
}

void Object::ApplyForce(Force* force) {
	// ignore certain types of forces
	if (	(!properties.feels_gravity && force->GetType() == FORCE_GRAVITY) ||
				(!properties.feels_user_input && force->GetType() == FORCE_INPUT) ||
				(!properties.feels_friction && force->GetType() == FORCE_FRICTION) )
		return;
	else
		accel += force->GetForce(this) / mass;
}

void Object::ResetForNextFrame() {
	old_pos = pos;
	accel.Clear();
	d.up = d.down = d.left = d.right = 0;

	// setup new bounding box
	bbox.setx1(pos.GetX());
	bbox.sety1(pos.GetY());
	bbox.setx2(pos.GetX() + GetWidth());
	bbox.sety2(pos.GetY() + GetHeight());

	bbox.Fix();

	assert(bbox.getx1() <= bbox.getx2());
	assert(bbox.gety1() <= bbox.gety2());
}

//! Solve for new position based on velocity
Vector2D Object::Solve() {

	vel += accel;
	pos += vel;

	if (debug_flag)
		fprintf(stderr, "vel=(%f,%f)\n", vel.GetX(), vel.GetY());
	
	UpdateProjectionRect();

	/*if (debug && properties.is_player) {
		fprintf(stderr, "-- YPOS  : %f\n", pos.GetY());
		fprintf(stderr, "-- YPOS-H: %f\n", pos.GetY() - GetHeight());
	}*/

	return pos;
}

void Object::Shutdown() {

	layer->RemoveObject(this);

	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		animations[i]->Shutdown();
		delete animations[i];
		animations[i] = NULL;
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
	layer = NULL;

	// just in case..
	is_dead = true;
}

Object::Object() {
	currentSprite = NULL;
	currentAnimation = NULL;
	flip_x = false; 
	is_dead = true;
	mass = 1.0f;
	simulation = NULL;
	debug_flag = false;
	pos.SetX(0); pos.SetY(0);
	old_pos.SetX(0); old_pos.SetY(0);
	accel.SetX(0); accel.SetY(0);
	vel.SetX(0); vel.SetY(0);
}

// A static helper function to load animations
bool Object::LoadAnimations(XMLNode &xDef, AnimationMapping *animation_lookup) {
	int i, iterator, max;

	Animation* anim = NULL;
	CString anim_name;
	XMLNode xAnim, xAnims;
	
	xAnims = xDef.getChildNode("animations");
	max = xAnims.nChildNode("animation");
	
	animations.resize(max);

	for (i=iterator=0; i<max; i++) {
		xAnim = xAnims.getChildNode("animation", &iterator);
		anim_name = xAnim.getAttribute("name");
		
		anim = Animation::New(GetGameState(), xAnim);

		if (!anim) {
			return false;
		} else {
			// if we have animation names (e.g. "walking") then use them to figure
			// out which index we store this animation at
			// if not, just put it in the next available index
			int index;
			
			if (animation_lookup)
				index = (*animation_lookup)[anim_name];
			else 
				index = i;
			
			animations[index] = anim;
		}
	}

	// set the default animation XXX error check? how?
	CString default_name;
	int default_index; 

	if (!animation_lookup) {
		currentAnimation = animations[0];
	} else { 
		default_name = xAnims.getAttribute("default");
		default_index = (*animation_lookup)[default_name];
		currentAnimation = animations[default_index];
	}

	// set the current sprite to the first frame of the animation
	currentSprite = currentAnimation->GetCurrentSprite();

	return true;
}

//! Load any sounds specified in the XML
bool Object::LoadSounds(XMLNode &xDef) {
				
	GameSound* sound = GetGameState()->GetSound();

	if (xDef.nChildNode("sounds")) {
		XMLNode xSounds = xDef.getChildNode("sounds");
		if (!sound->LoadSounds(xSounds))
			return false;
	}

	return true;
}

//! Load some common object properties from XML
bool Object::LoadProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	if (xProps.nChildNode("mass"))
		sscanf(xProps.getChildNode("mass").getText(), "%f", &mass);	
	
	ClearProperties(properties);
	properties.feels_gravity = 		xProps.nChildNode("affectedByGravity"); 
	properties.feels_user_input =	xProps.nChildNode("affectedByInput1"); 
	properties.feels_friction =		xProps.nChildNode("affectedByFriction"); 

	properties.is_solid =		xProps.nChildNode("solidObject");
	
	if (xProps.nChildNode("isOverlay")) {
		properties.is_overlay = 1;
	}

	if (xProps.nChildNode("springStrength")) {
		properties.spring_strength=xProps.getChildNode("springStrength").getInt();
	}

	return true;
}

// Return a vector with x,y set to 
// the closest these two objects can get to
// each other without colliding
//
// This method is a work in progress, and fairly nasty at the moment.
CollisionDirection Object::GetBound(Object* obj, Vector2D &v) {
	
	int debug = GetGameState()->GetGameOptions()->GetDebugMessageLevel();

	bool check_up = false, check_right = false;
	CollisionDirection d; d.up = d.down = d.left = d.right = 0;

	v.SetX(GetX());
	v.SetY(GetY()); 
	
	if (vel.GetX() > 0) check_right = true;
	if (vel.GetY() > 0) check_up = true;

	/*if (check_right && 
			old_pos.GetX() + (float)GetWidth() <= obj->pos.GetX() &&
			obj->pos.GetX() <= pos.GetX() + GetWidth() 
			) {
		d.right = 1;
	} else if (	GetX() <= 
							obj->GetX() + obj->GetWidth() && 
							obj->GetX() + obj->GetWidth() <=
							old_pos.GetX()) {
		d.left = 1;
	}*/

	/*if (check_up && 
		old_pos.GetY() <= obj->GetY() &&
		obj->GetY() <= GetY() + GetHeight()) {
		d.up = 1;
	} else*/ /*if (	old_pos.GetY() - GetHeight() >= 
							obj->GetY() && 
							obj->GetY() >=
							GetY() - GetHeight() ) {
		d.down = 1;
	}*/

	/*if (check_up && 
			old_pos.GetY() + Get() <= obj->GetX() &&
			obj->GetX() <= GetX() + GetWidth() 
			) {
		d.right = 1;*/

	//Rect o1 = projRect;
	//Rect o2 = obj->GetProjectionRect();

	float o1 = old_pos.GetY() - (float)GetHeight();
	float o2 = obj->pos.GetY();
	float o3 = pos.GetY() - (float)GetHeight();

	if ( check_up && old_pos.GetY() >= 
							obj->pos.GetY() - (float)obj->GetHeight() && 
							obj->pos.GetY() - (float)obj->GetHeight() >=
							pos.GetY()) {
		d.up = 1;
	} else if (	!check_up //&&
				/*old_pos.GetY() - (float)GetHeight() >= obj->pos.GetY() &&
				obj->pos.GetY() >= pos.GetY() - (float)GetHeight()*/ 
				// projRect.gety()
				) {
		d.down = 1;
	}

	if (debug && properties.is_player)
		fprintf(stderr, "     points(y)       \n"//BOT(y-h)\n"
										"OLD[1]:    %f           \n"//%f\n"
										"BOX[2]:    %f           \n"//%f\n"
										"NEW[3]:    %f           \n"//%f\n"
					 					"\n\n1 >= 2 >= 3\n\nCollisions:", 
			//old_pos.GetY(), old_pos.GetY() - GetHeight(),
			//obj->pos.GetY(), obj->pos.GetY() - obj->GetHeight(),
			//(float)GetY(), (float)GetY() - (float)GetHeight()
			o1, o2, o3
			);

	if (d.up) {
		v.SetY(obj->GetY() - GetHeight());
		if (debug) fprintf(stderr, "up!");
	}

	if (d.down) {
		v.SetY(obj->GetY() + obj->GetHeight() );
		if (debug) fprintf(stderr, "down / %f!", v.GetY());
	}

	if (d.left) {
		v.SetX(obj->GetX() + obj->GetWidth());
		if (debug) fprintf(stderr, "left!");
	}

	if (d.right) {
		v.SetX(obj->GetX() - GetWidth());
		if (debug) fprintf(stderr, "right!");
	}

	if (!(d.right || d.left || d.down || d.up))
		if (debug) fprintf(stderr, "NONE! Not good.\n");

	return d;
}
	
// get a rectangle whose area encompasses the total 
// space we moved from last frame to this frame
void Object::UpdateProjectionRect() {
	// compute a bounding box based on our current position and velocity
	Vector2D projection;

	// project the velocity vector backwards
	projection = vel;//;.Negation();
	//projection = vel;
	//projection.SetY(-projection.GetY());
	
	// if (properties.is_player)
		// fprintf(stderr, "v=(%f,%f)\n", projection.GetX(), projection.GetY());

	// get the projection rectangle
	projRect = bbox.Project(projection);
}

// rough, fast collision detection phase
bool const Object::IsColliding(Object *obj) {
	if (obj->properties.is_player) {
		//projRect.print();
		//obj->projRect.print();
	}
	return projRect.Overlaps(obj->GetProjectionRect());
}

void Object::MoveBack() {
	pos = old_pos;
}

void Object::Collide(Object* obj) {
	// default is no action, this is overriden in higher classes
}

void Object::MoveToNewPosition() {
	pos = Solve();
}

Object::~Object() {}
