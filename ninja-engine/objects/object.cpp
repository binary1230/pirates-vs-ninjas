#include "stdafx.h"
#include "object.h"

#include "gameState.h"
#include "window.h"
#include "globals.h"
#include "animation.h"
#include "animations.h"
#include "gameWorld.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "objectLayer.h"
#include "sprite.h"
#include "physics.h"
#include "objectFactory.h"

bool Object::debug_draw_bounding_boxes = 0;
map<std::string, Object*> Object::objectProtoTable;

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
	UpdatePositionFromPhysicsLocation();
	UpdateRotation();
}

void Object::UpdateRotation() {
	float physics_angle = _physics_body ? _physics_body->GetAngle() : 0.0f;

	if (physics_angle != 0.0f) {
		drawing_rotation_angle = RAD_TO_DEG(-physics_angle);
	} else {
		drawing_rotation_angle += _RotateVelocity;
	}
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

void Object::InitPhysics()
{

	if (!uses_physics_engine)
		return;

	if (_physics_body)
	{
		TRACE("WARNING: physics already initialized for this object, skipping.");
		return;
	}

	if (!PHYSICS)
	{
		TRACE("ERROR: Physics subsystem not yet initialized! Can't init object physics!");
		assert(PHYSICS);
		return;
	}

	// TODO: remove hardcoded junk here
	float fDensity = 0.1f;

	if (is_static)
		_physics_body = PHYSICS->CreateStaticPhysicsBox(_Pos.x, _Pos.y, GetWidth(), GetHeight(), is_sensor);
	else
		_physics_body = PHYSICS->CreateDynamicPhysicsBox(_Pos.x, _Pos.y, GetWidth(), GetHeight(), ignores_physics_rotation, fDensity, use_angled_corners_collision_box);

	_physics_body->SetUserData(this);
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

void Object::Clear() {
	_dont_draw = false;
	m_animationMapping.clear();
	m_bDrawBoundingBox = false;

	uses_physics_engine = false;
	is_static = false;
	is_sensor = false;
	ignores_physics_rotation = false;
	use_angled_corners_collision_box = false;
	is_overlay = false;

	is_dead = false;
	fade_out_time_total = fade_out_time_remaining = 0;
	is_fading = false;
	alpha = 255;
	display_time = -1;
	drawing_rotation_angle = _RotateVelocity = 0.0f;
	b_box_offset_x = b_box_offset_y = 0;
	b_box_width = b_box_height = 0;
	_Layer = NULL;
	_Pos.x = _Pos.y = 0.0f;
	m_kCurrentCollision.down = 0;
	m_kCurrentCollision.up = 0;
	m_kCurrentCollision.left = 0;
	m_kCurrentCollision.right = 0;
	currentAnimation = NULL;
	animations.clear();
	currentSprite = NULL;	
	flip_x = flip_y = false;
	_ObjectDefName = "";
	alpha = 255;
	b_box_offset_x = b_box_offset_y = 0;
	m_bDrawBoundingBox = false;
	_physics_body = NULL;
	unique_id = Object::next_object_id++;
	_DebugFlag = false;
}

bool Object::BaseInit() {
	return true;
}

void Object::ResetVolatileState(VolatileStateLevel level) {}

void Object::Draw() {
	assert(WORLD != NULL);

	if (_dont_draw)
		return;

	int flip_offset_x = -b_box_offset_x;
	int flip_offset_y = 0;

	if (b_box_offset_x)
	{
		if (flip_x)
			flip_offset_x = - currentAnimation->GetWidth() + b_box_offset_x + GetWidth();

		flip_offset_y = b_box_height; // not sure if this is right
	}

	DrawAtOffset(flip_offset_x, flip_offset_y);
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
	x = (int)_Pos.x + offset_x;
	y = (int)_Pos.y + offset_y;

	// take into account the camera now.
	if (!is_overlay)
		WORLD->TransformWorldToView(x, y, _Layer->GetScrollSpeed());
	
	// compute absolute x,y coordinates on the screen
	y = y + GetHeight();
	WORLD->TransformViewToScreen(x, y);
}

// Same as Transform(), just for rectangles only.
void Object::TransformRect(_Rect &r) {

	int x1, x2, y1, y2, w, h;
	
	x1 = (int)r.getx1();	
	y1 = (int)r.gety1();	
	x2 = (int)r.getx2();
	y2 = (int)r.gety2();
	w = x2 - x1;
	h = y2 - y1;

	// take into account the camera now.
	if (!is_overlay) {
		WORLD->TransformWorldToView(x1, y1, _Layer->GetScrollSpeed());
		
		// old way: this would result in bounding boxes for objects on layers with different scroll speeds
		// having the wrong width/height.  still, might be useful in certain situations later, re-enabl if needed.
		// WORLD->TransformWorldToView(x2, y2, _Layer->GetScrollSpeed());

		// new way: ignore scroll speed and just use width and height directly. don't modify width/height
		x2 = x1 + w;
		y2 = y1 + h;
	}
	
	// compute absolute x,y coordinates on the screen
	WORLD->TransformViewToScreen(x1, y1);
	WORLD->TransformViewToScreen(x2, y2);

	r.set(x1,y1,x2,y2);
}

//! This function does the real dirty work of drawing.
void Object::DrawAtOffset(int offset_x, int offset_y, Sprite* sprite_to_draw) 
{	
	int x, y;
	Transform(x, y, offset_x, offset_y);
				
	// draw for real
	if (!sprite_to_draw)
		sprite_to_draw = currentSprite;

	if (sprite_to_draw)
		WINDOW->DrawSprite(sprite_to_draw, x, y, flip_x, flip_y, drawing_rotation_angle, alpha);

	#if DEBUG_DRAW_SPRITE
	if (sprite_to_draw && (b_box_offset_x || b_box_offset_y))
	{
		const bool bOnlyDrawBoundingBox = true;
		WINDOW->DrawSprite(sprite_to_draw, x, y, flip_x, flip_y, drawing_rotation_angle, alpha, bOnlyDrawBoundingBox);
	}
	#endif

	if (m_bDrawBoundingBox) 
	{
		_Rect bbox_t;

		// get current bounding box
		bbox_t.set(	_Pos.x, _Pos.y, _Pos.x + GetWidth(), _Pos.y + GetHeight());

		// draw current bounding rectangle, pink
		TransformRect(bbox_t);
		WINDOW->DrawRect(bbox_t, _bounding_box_color);
	}
}

int Object::GetWidth() const
{
	if (b_box_width > 0) {
		return b_box_width;
	}

	if (animations.size() <= 0 || !animations[0]) {
		return 0;
	}

	return animations[0]->GetWidth();
}

int Object::GetHeight() const
{
	if (b_box_height > 0) {
		return b_box_height;
	}

	if (animations.size() <= 0 || !animations[0]) {
		return 0;
	}
	return animations[0]->GetHeight();
}

b2Vec2 Object::GetCenter() const
{
	return b2Vec2(_Pos.x + GetWidth() / 2, _Pos.y + GetHeight() / 2);
}

void Object::ResetForNextFrame()
{
	m_kCurrentCollision.up = m_kCurrentCollision.down = m_kCurrentCollision.left = m_kCurrentCollision.right = 0;

	if (_physics_body)
	{
		const b2Vec2& kPos = _physics_body->GetPosition();
		_Pos.x = kPos.x;
		_Pos.y = kPos.y;
	}
}

void Object::BaseShutdown() {
	assert(_Layer);
	_Layer->RemoveObject(this);

	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		if (animations[i])
			animations[i]->Shutdown();
		SAFE_DELETE(animations[i]);
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
	_Layer = NULL;
	is_dead = true;
	display_time = -1;

	if (PHYSICS && _physics_body)
		PHYSICS->RemoveFromWorld(_physics_body);
}

unsigned long Object::next_object_id = 0;

void Object::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold)
{
	// default is no action, this is overidden in higher classes
}

void Object::PlayAnimation( uint uiIndex )
{
	assert(uiIndex >=0 && uiIndex < animations.size() && "Animation index out of range.");

	if (uiIndex < 0 || uiIndex >= animations.size())
	{
		TRACE("ERROR: Animation #%i is out of range! Can't switch to it.\n", uiIndex);
		return;
	}

	if (!animations[uiIndex])
	{
		TRACE("ERROR: Animation #%i contains nothing! Can't switch to it.\n", uiIndex);
		return;
	}

	if (currentAnimation == animations[uiIndex])
		return;

	currentAnimation = animations[uiIndex];
	currentAnimation->ResetAnimation();
}

void Object::ApplyImpulse( float x, float y )
{
	ApplyImpulse(b2Vec2(x, y));
}

void Object::ApplyImpulse(const b2Vec2& v)
{
	if (_physics_body)
		_physics_body->ApplyLinearImpulseToCenter(v, true);
}

void Object::UpdatePositionFromPhysicsLocation()
{
	if (!_physics_body)
		return;

	_Pos.x = METERS_TO_PIXELS(_physics_body->GetPosition().x) - float(GetWidth()) / 2;
	_Pos.y = METERS_TO_PIXELS(_physics_body->GetPosition().y) - float(GetHeight()) / 2;
}

bool Object::FinishLoading()
{
	string objectDefName = GetPropObjectDefName();
	XMLNode* xDef = OBJECT_FACTORY->FindObjectDefinition(objectDefName);
	if (!xDef) {
		TRACE("Can't find object def named: %s", objectDefName.c_str());
		return false;
	}
	assert(xDef);
	return LoadFromObjectDef(*xDef);
}

bool Object::LoadFromObjectDef(XMLNode& xDef) {
	SetPropObjectDefName(xDef.getAttribute("name"));

	if (!Init())
		return false;

	if (!LoadObjectProperties(xDef))
		return false;

	if (!LoadObjectSounds(xDef))
		return false;

	if (!LoadObjectAnimations(xDef))
		return false;

	InitPhysics();

	return true;
}

bool Object::LoadObjectSounds(XMLNode &xDef) {
	if (xDef.nChildNode("sounds")) {
		XMLNode xSounds = xDef.getChildNode("sounds");
		if (!SOUND->LoadSounds(xSounds))
			return false;
	}

	return true;
}

bool Object::LoadObjectProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	// NOTE: These properties override anything previously unserialized from savefile

	is_static = xProps.nChildNode("solidObject") != 0;
	is_sensor = xProps.nChildNode("sensorOnly") != 0;

	uses_physics_engine = is_static || is_sensor;

	is_overlay = xProps.nChildNode("isOverlay") != 0;
	
	if (xProps.nChildNode("boundingBox") != 0)
	{
		XMLNode xBoundingBox = xProps.getChildNode("boundingBox");

		if (!xBoundingBox.nChildNode("offset_x"))
		{
			return false;
		}

		if (!xBoundingBox.getChildNode("offset_x").getInt(b_box_offset_x) ||
			!xBoundingBox.getChildNode("offset_y").getInt(b_box_offset_y) ||
			!xBoundingBox.getChildNode("width").getInt(b_box_width) ||
			!xBoundingBox.getChildNode("height").getInt(b_box_height))
		{
			TRACE("Invalid bounding box info.\n");
			return false;
		}
	}

	return true;
}

// A helper function to load animations
bool Object::LoadObjectAnimations(XMLNode &xDef) {
	uint i;
	int num_xml_animations, num_animation_slots_needed = -1, iterator;

	Animation* anim = NULL;
	std::string anim_name;
	XMLNode xAnim, xAnims;

	xAnims = xDef.getChildNode("animations");
	num_xml_animations = xAnims.nChildNode("animation");

	if (m_animationMapping.size())
		num_animation_slots_needed = m_animationMapping.size();

	animations.resize(max(num_xml_animations, num_animation_slots_needed));

	// zero out all the animations to NULL
	for (i = 0; i < animations.size(); ++i)
		animations[i] = NULL;

	// read everything from XML
	for (i = iterator = 0; i<(uint)num_xml_animations; ++i)
	{
		xAnim = xAnims.getChildNode("animation", &iterator);
		anim_name = xAnim.getAttribute("name");

		// Load the animation	
		anim = Animation::Load(xAnim, this);

		if (!anim)
			return false;

		// if we have animation names (e.g. "walking") then use them to figure
		// out which index we store this animation at
		// if not, just put it in the next available index
		uint index;
		if (m_animationMapping.size())
			index = m_animationMapping[anim_name];
		else
			index = i;

		assert(index >= 0 && index < animations.size());
		animations[index] = anim;
	}
	
	std::string default_name;
	int default_index;

	if (m_animationMapping.size()) {
		default_name = xAnims.getAttribute("default");
		default_index = m_animationMapping[default_name];
		currentAnimation = animations[default_index];
	} else {
		if (animations.size() > 0) {
			currentAnimation = animations[0];
		}
	}

	if (currentAnimation) {
		currentSprite = currentAnimation->GetCurrentSprite();
	}

	return true;
}

Object::Object() {
	Clear();
}
Object::~Object() {}

bool Object::ContainsPoint(const b2Vec2 & p) const
{
	// point p must be in layer-space (i.e. already adjusted for layer scroll speed)

	// undo effects of scroll speed on width/height [effectively, makes the width/height way bigger]
	float adjustedWidth  = GetWidth()  / GetLayer()->GetScrollSpeed();
	float adjustedHeight = GetHeight() / GetLayer()->GetScrollSpeed();

	return  p.x >= _Pos.x && p.x <= _Pos.x + adjustedWidth &&
			p.y >= _Pos.y && p.y <= _Pos.y + adjustedHeight;
}

Object* Object::AddPrototype(string type, Object * obj)
{
	TRACE("adding object prototype for '%s' ", type.c_str());
	objectProtoTable[type] = obj;
	return obj; // handy
}

Object* Object::CreateObject(std::string type)
{
	std::map<string, Object*>::iterator it;
	it = objectProtoTable.find(type);
	if (it == objectProtoTable.end()) {
		TRACE("CreateObject(): Object type named '%s' not found", type.c_str());
		return NULL;
	}

	Object* obj = it->second;
		
	return obj->Clone();
}

BOOST_CLASS_EXPORT_GUID(Object, "Object")