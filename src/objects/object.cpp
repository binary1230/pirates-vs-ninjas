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
	level_width  = simulation->GetWidth();
	level_height = simulation->GetHeight();
}

void Object::UpdateFade() {
	
}

void Object::FadeOut(int time) {
	fade_out = time;
	is_fading = true;
}

bool Object::BaseInit() {
	SetupCachedVariables();
	ClearProperties(properties);
	is_dead = false;
	fade_out = 0;
	is_fading = false;
	alpha = 255;
	old_pos = pos;
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
	
	UpdateProjectionRectFromVelocity();

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

// Return a vector with x,y set to 
// the closest these two objects can get to
// each other without colliding
// XXX BIG MESS and NOT FINISHED, not even close.
CollisionDirection Object::GetBound(Object* obj, Vector2D &v) {
	
	int debug = GetGameState()->GetGameOptions()->GetDebugMessageLevel();

	bool check_up = false, check_right = false;
	CollisionDirection d; d.up = d.down = d.left = d.right = 0;

	v.SetX(GetX());
	v.SetY(GetY()); 
	
	if (vel.GetX() > 0) check_right = true;
	if (vel.GetY() > 0) check_up = true;
	
	float top = projRect.gety2();
	float mid = obj->GetY() + obj->GetHeight();
	float bot = projRect.gety1();

	// handle up-down collisions
	if (!check_up && top >= mid && mid >= bot) {
		//if (vel.GetY() >= 0)
		//	d.up = 1;
		//else 
			d.down = 1;
	}

	//debug = true;

	/*if (d.up) {
		v.SetY(obj->GetY() - GetHeight());
		if (debug) fprintf(stderr, "up!");
	}*/

	if (d.down) {
		v.SetY(obj->GetY() + obj->GetHeight() );
		if (debug) fprintf(stderr, "down!");
	}

	if (d.left) {
		v.SetX(obj->GetX() + obj->GetWidth());
		if (debug) fprintf(stderr, "left!");
	}

	if (d.right) {
		v.SetX(obj->GetX() - GetWidth());
		if (debug) fprintf(stderr, "right!");
	}

	//if (!(d.right || d.left || d.down || d.up))
	//	if (debug) fprintf(stderr, "NONE! Not good.");

	// fprintf(stderr, "\n");

	return d;
}
	
// get a rectangle whose area encompasses the total 
// space we moved from last frame to this frame
// based solely on velocity (not collisions)
void Object::UpdateProjectionRectFromVelocity() {
	projRect = bbox.Project(vel);
}

void Object::UpdateProjectionRectFromCollisions(Vector2D &newPos) {
	/*projRect.setx1(fmin(newPos.GetX(), old_pos.GetX()));
	projRect.setx2(fmax(newPos.GetX(), old_pos.GetX()));
	projRect.sety1(fmin(newPos.GetY(), old_pos.GetY()));
	projRect.sety2(fmax(newPos.GetY(), old_pos.GetY()));
	projRect.Fix();*/
}

// rough, fast collision detection phase
bool const Object::IsColliding(Object *obj) {
	if (obj->properties.is_player) {
		//projRect.print();
		//obj->projRect.print();
	}
	return projRect.Overlaps(obj->GetProjectionRect());
}

void Object::Collide(Object* obj) {
	// default is no action, this is overriden in higher classes
}

void Object::MoveToNewPosition() {
	pos = Solve();
}

Object::~Object() {}
