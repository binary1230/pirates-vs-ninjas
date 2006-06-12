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

// GetWidth() and GetHeight() need rethinking.

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
	simulation->TransformViewToScreen(x, y);
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

	if (properties.is_player || properties.is_solid)
		GetGameState()->GetWindow()->
		DrawRect(x, y, GetWidth(), GetHeight());
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
}

//! Solve for new position based on velocity
Vector2D Object::Solve() {

	vel += accel;
	pos += vel;

	if (debug_flag)
		fprintf(stderr, "vel=(%f,%f)\n", vel.GetX(), vel.GetY());

	return pos;
}

void Object::Shutdown() {
				
	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		animations[i]->Shutdown();
		delete animations[i];
		animations[i] = NULL;
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
}

Object::Object() {
	currentSprite = NULL;
	currentAnimation = NULL;
	flip_x = false; 
	mass = 1.0f;
	simulation = NULL;
	debug_flag = false;
	pos.SetX(0); pos.SetY(0);
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

	return true;
}

// Return a vector with x,y set to 
// the closest these two objects can get to
// each other without colliding
CollisionDirection Object::GetBound(Object* obj, Vector2D &v) {
	
	bool check_up = false, check_right = false;
	CollisionDirection d; d.up = d.down = d.left = d.right = 0;

	v.SetX(GetX());
	v.SetY(GetY()); 
	
	if (vel.GetX() > 0) check_right = true;
	if (vel.GetY() > 0) check_up = true;

	if (check_right && 
			old_pos.GetX() + GetWidth() <= obj->GetX() &&
			obj->GetX() <= GetX() + GetWidth() 
			) {
		d.right = 1;
	} else if (	GetX() <= 
							obj->GetX() + obj->GetWidth() && 
							obj->GetX() + obj->GetWidth() <=
							old_pos.GetX()) {
		d.left = 1;
	}

	if (check_up && 
		old_pos.GetY() <= obj->GetY() &&
		obj->GetY() <= GetY() + GetHeight()) {
		d.up = 1;
	} else if (	GetY() <= 
							obj->GetY() + obj->GetHeight() && 
							obj->GetY() + obj->GetHeight() <=
							old_pos.GetY() ) {
		d.down = 1;
	}

	if (d.up) {
		v.SetY(obj->GetY() - GetHeight());
		printf("up!");
	}

	if (d.down) {
		v.SetY(obj->GetY() + GetHeight() );
		printf("down / %f!", v.GetY());
	}

	if (d.left) {
		v.SetX(obj->GetX() + obj->GetWidth());
		printf("left!");
	}

	if (d.right) {
		v.SetX(obj->GetX() - GetWidth());
		printf("right!");
	}

	printf("-\n");

	return d;
}

// XXX need to check OLDPOS too in order to form
// a path where the player went.  otherwise, they could
// 'skip' through a block if going too fast
bool Object::IsColliding(Object *obj) {
	if (GetX() + GetWidth() > obj->pos.GetX() &&
			GetX() < obj->pos.GetX() + obj->GetWidth() &&
			GetY() + GetHeight() > obj->pos.GetY() &&
			GetY() < obj->pos.GetY() + obj->GetHeight() )
			return true;

	return false;
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
