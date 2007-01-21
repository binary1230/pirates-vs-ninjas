#include "object.h"

#include "gameState.h"
#include "window.h"
#include "force.h"
#include "globals.h"
#include "animation.h"
#include "animations.h"
#include "physSimulation.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "objectLayer.h"
#include "sprite.h"

bool Object::debug_draw_bounding_boxes = 0;

void Object::SetObjectDefName(const char* _name) {
	SAFE_DELETE(objectDefName);
	objectDefName = new CString(_name);
}

// Used as criteria for STL find()
bool ObjectIsDead(Object* obj) {
	assert(obj != NULL);
	return obj->IsDead();
}

// Objects can call this if they use
// simple animations.
void Object::UpdateSimpleAnimations() {
	if (!currentAnimation)
		return;

	currentAnimation->Update();
	currentSprite = currentAnimation->GetCurrentSprite();
}

// Base update stuff used by all objects
void Object::BaseUpdate() {
	UpdateDisplayTime();
	UpdateFade();

	if (use_rotation)
		rotate_angle += rotate_velocity;
}

// Decrement the display time, when it reaches 0, we 
// destroy this object. If it's -1, then it lives forever
void Object::UpdateDisplayTime() {
	if (display_time == -1)
		return;

	if (display_time > 0)
		display_time--;

	if (display_time == 0)
		is_dead = true;
}

//! Cache some commonly used stuff
// ('cause the profiler says so!)
void Object::SetupCachedVariables() {
	assert(WORLD != NULL);
	level_width  = WORLD->GetWidth();
	level_height = WORLD->GetHeight();
	
	if (animations.size() > 0 && animations[0]) {
		width = animations[0]->GetWidth();
		height = animations[0]->GetHeight();
	} else {
		width = 0;
		height = 0;
	}
}

void Object::UpdateFade() {
	if (!is_fading)
		return;

	if (!fade_out_time_remaining) {
		is_fading = false;
	} else {
		--fade_out_time_remaining;
	}
	
	alpha = uint(	( (float)fade_out_time_remaining / (float)fade_out_time_total)
								* 255.0f);
}

void Object::FadeOut(int time) {
	fade_out_time_total = fade_out_time_remaining = time;
	alpha = 255;
	is_fading = true;
}

bool Object::BaseInit() {
	LogObjectEvent(OBJECT_INIT);
	tmp_debug_flag = 0;
	ClearProperties(properties);
	is_dead = false;
	fade_out_time_total = fade_out_time_remaining = 0;
	is_fading = false;
	alpha = 255;
	old_pos = pos;
	display_time = -1;
	width = height = 0;
	controller_num = 0;
	level_width = 0;
	level_height = 0;
	rotate_angle = rotate_velocity = 0.0f;
	use_rotation = false;
	return true;
}

void Object::Draw() {
	assert(WORLD != NULL);

	if (tmp_debug_flag)
		fprintf(stderr, "DEBUG FLAG!!\n");

	DrawAtOffset(0,0);
}

//! Ultimately we need the actual, on-screen coordinates of where
//! to draw the sprite.  To get to those from the object's "world" coordinates
//! as computed by the physics engine, we need to take into account the 
//! position of the camera, and we need to flip the Y axis.  These
//! things are handled by the WORLD->TransformXXX() methods.
//
//! This function populates x,y (reference params) with the 
//! correctly transformed coordinates.
void Object::Transform(int &x, int &y, const int &offset_x, const int &offset_y) {
	x = (int)pos.GetX() + offset_x;
	y = (int)pos.GetY() + offset_y;

	// take into account the camera now.
	if (!properties.is_overlay)
		WORLD->TransformWorldToView(x, y);
	
	// compute absolute x,y coordinates on the screen
	y = y + GetHeight();
	WORLD->TransformViewToScreen(x, y);
}

// Same as Transform(), just for rectangles only.
void Object::TransformRect(_Rect &r) {

	int x1, x2, y1, y2, w, h;
	
	// r.Fix();
	
	x1 = (int)r.getx1();	
	y1 = (int)r.gety1();	
	x2 = (int)r.getx2();
	y2 = (int)r.gety2();
	w = x2 - x1;
	h = y2 - y1;

	// take into account the camera now.
	if (!properties.is_overlay) {
		WORLD->TransformWorldToView(x1, y1);
		WORLD->TransformWorldToView(x2, y2);
	}
	
	// compute absolute x,y coordinates on the screen
	WORLD->TransformViewToScreen(x1, y1);
	WORLD->TransformViewToScreen(x2, y2);

	r.set(x1,y1,x2,y2);
	/*r.setx1(x1); 	r.sety1(y1);
	r.setx2(x2); 	r.sety2(y2);*/
	
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
		WINDOW->DrawSprite(sprite_to_draw, x, y, flip_x, flip_y, use_rotation, rotate_angle, alpha);

	if (!debug_draw_bounding_boxes)
		return;

	_Rect bbox_t;
	_Rect projRect_t = projRect;
	_Rect bbox_t_old = bbox;

	// get current bounding box
	bbox_t.set(	pos.GetX(), pos.GetY(), 
							pos.GetX() + GetWidth(), pos.GetY() + GetHeight()
							);

	// draw projection rectangle, blue
	TransformRect(bbox_t);
	TransformRect(bbox_t_old);
	TransformRect(projRect_t);

	if (	properties.is_player || 
				properties.is_solid || 
				properties.is_collectable) {

		// draw old bounding rectangle, dark pink
		WINDOW->DrawRect(bbox_t_old, makecol(127,0,127));
	
		// draw current bounding rectangle, pink
		WINDOW->DrawRect(bbox_t, makecol(255,0,255));

		// draw projection rectangle, blue
		WINDOW->DrawRect(projRect_t, makecol(0, 0, 255));
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

	bbox.set(		pos.GetX(), pos.GetY(), 
							pos.GetX() + GetWidth(), pos.GetY() + GetHeight()
							);

	// assert(bbox.getx1() <= bbox.getx2());
	// assert(bbox.gety1() <= bbox.gety2());
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

// static: Returns true if this type of object is able to collide with anything
bool Object::CanCollide(Object* obj) { 
	assert(obj != NULL);
	return 	obj->properties.is_solid ||
					obj->properties.is_player || 
					obj->properties.is_spring ||
					obj->properties.is_collectable || 
					obj->properties.is_fan ||
					obj->properties.is_door ||
					obj->properties.is_ring ||
					obj->properties.is_ball;
}

void Object::BaseShutdown() {
	LogObjectEvent(OBJECT_SHUTDOWN);

	layer->RemoveObject(this);

	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		animations[i]->Shutdown();
		SAFE_DELETE(animations[i]);
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
	layer = NULL;
	is_dead = true;
	display_time = -1;

	SAFE_DELETE(objectDefName);
}

unsigned long Object::debug_object_id = 0;

Object::Object() {
	LogObjectEvent(OBJECT_NEW);
	objectDefName = NULL;
	unique_id = Object::debug_object_id++;
	layer = NULL;
	currentSprite = NULL;
	currentAnimation = NULL;
	flip_x = false; 
	flip_y = false; 
	is_dead = true;
	mass = 1.0f;
	debug_flag = false;
	pos.SetX(0); pos.SetY(0);
	old_pos.SetX(0); old_pos.SetY(0);
	accel.SetX(0); accel.SetY(0);
	vel.SetX(0); vel.SetY(0);
	display_time = -1;
	rotate_angle = rotate_velocity = 0.0f;
	use_rotation = false;
}

// Return a vector with x,y set to 
// the closest these two objects can get to
// each other without colliding
// XXX BIG MESS and NOT FINISHED, not even close.
CollisionDirection Object::GetBound(Object* obj, Vector2D &v) {
	
	int debug = OPTIONS->GetDebugMessageLevel();

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
	projRect = bbox;
	projRect.Project(vel);
}

void Object::UpdateProjectionRectFromCollisions(Vector2D &newPos) {
	/*projRect.setx1(fmin(newPos.GetX(), old_pos.GetX()));
	projRect.setx2(fmax(newPos.GetX(), old_pos.GetX()));
	projRect.sety1(fmin(newPos.GetY(), old_pos.GetY()));
	projRect.sety2(fmax(newPos.GetY(), old_pos.GetY()));
	projRect.Fix();*/
}

// rough, fast collision detection phase
bool Object::IsColliding(Object *obj) const {
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

Object::~Object() {
	LogObjectEvent(OBJECT_DELETE);
	SAFE_DELETE(objectDefName);
}
