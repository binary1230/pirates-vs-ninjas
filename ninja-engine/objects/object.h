#ifndef __OBJECT_H
#define __OBJECT_H

#include "globals.h"

#include "rect.h"
#include "animations.h"
#include "objectLayer.h"
#include "physics.h"

// class Object;
class Animation;
class Sprite;
class ObjectFactory;
class b2Body;
class Editor;

struct CollisionDirection {
	unsigned up : 1;
	unsigned down : 1;
	unsigned left : 1;
	unsigned right : 1;
};

enum VolatileStateLevel {
	LEVEL_ITEMS,
	LEVEL_PLAYERS,
};

//! Various properties of an Object
struct ObjectProperties 
{
	// NOTE: If you add anything here, update ClearProperties()
	bool feels_gravity;	
	bool feels_user_input;
	bool feels_friction;

	// TEMP HACK - this object spawns enemies
	bool spawns_enemies;

	//! Whether we should register with the physics engine or not
	bool uses_physics_engine;

	//! If static, this object WILL NOT MOVE, ever.
	//! Only matters if uses_physics_engine is on
	bool is_static; 

	//! If set, this object will not get a callback if being collided with
	//! Note, however, that it will still deal out collision
	bool is_sensor;

	//! Don't rotate if physical. e.g. if we want to tip over, don't let us
	bool ignores_physics_rotation;

	//! Don't let physics touch our rotation, we'll do it ourselves
	bool do_our_own_rotation;

	//! Tell the physics engine to create angled corners for our bounding box 
	//! (useful to keep some objects from sticking on seams, like the player)
	bool use_angled_corners_collision_box;

	//! true if this object is an overlay
	//! e.g. not IN the world, but on top it,
	//! like our status bar or health or something.
	//! Overlays ignore camera information
	bool is_overlay;
};

namespace boost {
	namespace serialization {

		// custom serializable types for Boost

		template<class Archive>
		void serialize(Archive & ar, ObjectProperties& p, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(p.feels_gravity);
			ar & BOOST_SERIALIZATION_NVP(p.feels_user_input);
			ar & BOOST_SERIALIZATION_NVP(p.feels_friction);
			ar & BOOST_SERIALIZATION_NVP(p.spawns_enemies);
			ar & BOOST_SERIALIZATION_NVP(p.uses_physics_engine);
			ar & BOOST_SERIALIZATION_NVP(p.is_static);
			ar & BOOST_SERIALIZATION_NVP(p.is_sensor);
			ar & BOOST_SERIALIZATION_NVP(p.ignores_physics_rotation);
			ar & BOOST_SERIALIZATION_NVP(p.do_our_own_rotation);
			ar & BOOST_SERIALIZATION_NVP(p.use_angled_corners_collision_box);
			ar & BOOST_SERIALIZATION_NVP(p.is_overlay);
		}
	}
}

//! Clears property masks
inline void ClearProperties(struct ObjectProperties& p) {
	p.feels_gravity = 0;
	p.feels_user_input = 0;
	p.feels_friction = 0;
	p.is_overlay = 0;
	p.uses_physics_engine = 0;
	p.use_angled_corners_collision_box = 0;
	p.spawns_enemies = 0;
	p.is_static = 0;
	p.is_sensor = 0;
	p.ignores_physics_rotation = 0;
	p.do_our_own_rotation = 0;
}

#define IMPLEMENT_CLONE(TYPE) \
   Object* Clone() const { return new TYPE(/* *this */); }

#define MAKE_PROTOTYPE(TYPE) \
   Object* TYPE ## _myProtoype1 = Object::AddPrototype(#TYPE, new TYPE());

#define MAKE_PROTOTYPE_ALIAS(TYPE, NAME) \
   Object* TYPE ## _myProtoype2 = Object::AddPrototype(NAME, new TYPE());

//! A drawable entity in the world

//! Objects have physical properties associated with them, but do
//! not always have to take part in the world
class Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version)
	{
		ar & BOOST_SERIALIZATION_NVP(pos);
		ar & BOOST_SERIALIZATION_NVP(objectDefName);
		ar & BOOST_SERIALIZATION_NVP(controller_num);
		ar & BOOST_SERIALIZATION_NVP(properties);
		ar & BOOST_SERIALIZATION_NVP(m_pkLayer);

		if (file_version >= 2) {
			ar & BOOST_SERIALIZATION_NVP(_use_rotation);
			ar & BOOST_SERIALIZATION_NVP(_rotate_velocity);
		}
	}

	// implement "prototype pattern" for object creation
	static map<std::string, Object*> objectProtoTable;
	virtual Object* Clone() const = 0;

	protected:

		//! A pointer to the layer this object is on
		ObjectLayer* m_pkLayer;

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;
		
		//! Current position
		b2Vec2 pos;
		
		//! The directions of current collisions (up,down,right,left)
		CollisionDirection m_kCurrentCollision;
		
		//! Object properties
		struct ObjectProperties properties;
		
		//! Points to the current animation
		Animation* currentAnimation;

		//! Array of all loaded animations
		vector<Animation*> animations;

		//! Points to the sprite we should draw on next Draw()
		Sprite* currentSprite;

		//! Whether to flip the sprite when drawing
		bool flip_x;
		bool flip_y;

		bool _dont_draw;

		void Clear();
		bool BaseInit();	
		void BaseShutdown();

		//! True if this object is no longer in play and needs to be cleaned up
		bool is_dead;

		//! If true, this object can print debug info out if it wants to.
		bool debug_flag;

		//! How long, in frames, do we display this object.
		//! This value is decremented each frame.  When it reaches
		//! 0, the object is deleted.
		//! When set to -1, this value is ignored
		int display_time;

		std::string objectDefName;

		// -- UNUSUED BELOW --

		//! Current fade-out time
		int fade_out_time_total;
		int fade_out_time_remaining;

		//! True if the object is fading in or out
		bool is_fading;

		//! Alpha (transparency) of this object (0=inviz, 255=opaque)
		int alpha;

		// -- END UNUSED --

		//! Do common object updates
		void BaseUpdate();

		void UpdatePositionFromPhysicsLocation();

		virtual bool LoadFromObjectDef(XMLNode & xDef);
		bool LoadObjectSounds(XMLNode& xDef);
		virtual bool LoadObjectProperties(XMLNode& xDef);
		virtual bool LoadXMLInstanceProperties(XMLNode& xObj);
		bool LoadObjectAnimations(XMLNode& xDef);

		//! Update display times
		void UpdateDisplayTime();

		//! (optional) Do a simple update of the animations
		//! Most objects will call this
		void UpdateSimpleAnimations();

		//! Update the fading stuff
		void UpdateFade();

		//! optimization: cache Width and Height of the object
		// (we may need to rethink where these come from)
		// int width, height;

		//! Bounding box offsets from the bottom left of the first sprite
		// (maye need to play with these)
		int b_box_offset_x, b_box_offset_y;
		int b_box_width, b_box_height;

		//! Rotational parameters
		float rotate_angle, _rotate_velocity;
		bool _use_rotation;

		//! Whether to draw the bounding box or not
		bool m_bDrawBoundingBox;
		ALLEGRO_COLOR _bounding_box_color;

		b2Body* _physics_body;

		// loading-only paramaters
		AnimationMapping m_animationMapping;

		// A unqiue ID that is incremented every time an object is created.  
		static unsigned long next_object_id;
		unsigned long unique_id;
	
	public:
		// WRONG Protected constructor, this means we can't directly
		// instantiate Object's, we need to use a friend or derived class.
		Object();

		// Whether to draw ALL the different rectangles or not (DEBUG)
		static bool debug_draw_bounding_boxes;

		inline unsigned long GetID() const { return unique_id; }

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update() = 0;

		virtual void InitPhysics();

		// TODO: Make this take an animation code, for now it just takes the index of the animation 
		// as defined by the order we found them in the XML file.  Very prone to errors. HACKY
		virtual void PlayAnimation(uint uiIndex);

		void SetDrawBounds(bool bDrawBounds, ALLEGRO_COLOR color = al_map_rgb(255, 0, 255)) {
			m_bDrawBoundingBox = bDrawBounds;
			_bounding_box_color = color;
		}
		
		//! Fade this object out over a given time (in frames)
		void FadeOut(int time);

		virtual void Draw();

		void Transform(	int &x, int &y, const int &offset_x = 0, const int &offset_y = 0);
		void TransformRect(_Rect &r);

		inline void SetDisplayTime(int time) {
			display_time = time;
		}
		inline int GetDisplayTime() {
			return display_time;
		}
	
		//! Draw this object at its coordinates plus specified offset
		//! Optionally, you can pass in a specific sprite to draw, otherwise
		//! It'll just use the current sprite (most cases)
		void DrawAtOffset(int x, int y, Sprite* = NULL);	
		
		//! Functions to get/set position
		inline int GetPropX() const				{ return (int)pos.x; }
		inline int GetPropY() const				{ return (int)pos.y; }
		inline b2Vec2 GetXY() const { return pos; }; 

		inline void SetPropX(const int _x) { SetXY(b2Vec2(_x, pos.y)); }
		inline void SetPropY(const int _y) { SetXY(b2Vec2(pos.x, _y)); }
		inline void SetXY(const int _x, const int _y) {
			SetXY(b2Vec2(_x, _y));
		}
		inline void SetXY(const b2Vec2 &_pos) {
			pos = _pos;
			if (_physics_body)
				PHYSICS->UpdatePhysicsBodyPosition(_physics_body, pos.x, pos.y, GetWidth(), GetHeight());
		}

		inline int GetAlpha() { return alpha; };
		inline void SetAlpha(const int a) { alpha = a; };

		inline void SetFlipX(const bool val) { flip_x = val; };
		inline void SetFlipY(const bool val) { flip_y = val; };

		//! Functions to get/set velocity
		inline float GetVelX() 					{ 
			return _physics_body ? _physics_body->GetLinearVelocity().x : 0.0f; 
		}

		inline float GetVelY() 					{ 
			return _physics_body ? _physics_body->GetLinearVelocity().y : 0.0f; 
		}

		inline b2Vec2 GetVelXY() const { 
			return _physics_body ? _physics_body->GetLinearVelocity() : b2Vec2(0.0f, 0.0f); 
		}

		inline void SetVelX(const float _vx) 		{ 
			if (_physics_body)
				_physics_body->SetLinearVelocity(b2Vec2(_vx, _physics_body->GetLinearVelocity().y));
		}
		inline void SetVelY(const float _vy) 		{ 
			if (_physics_body)
				_physics_body->SetLinearVelocity(b2Vec2(_physics_body->GetLinearVelocity().x, _vy));
		}

		inline void SetVelXY(const float _vx, const float _vy) {
			if (_physics_body)
				_physics_body->SetLinearVelocity(b2Vec2(_vx, _vy));
		}

		inline void SetVelXY(const b2Vec2& v) {
			if (_physics_body)
				_physics_body->SetLinearVelocity(v); 
		}
		
		inline void SetVelRotate(const float vel) {
			_rotate_velocity = vel;
		}

		inline void SetUseRotation(const bool state) {
			_use_rotation = state;
		}

		//! Get width/height of this object
		int GetWidth() const;
		int GetHeight() const;
	
		//! Physics: reset this object's physics stuff for next frame
		void ResetForNextFrame();
		
		struct ObjectProperties GetProperties() const { return properties; };
		inline void SetProperties(struct ObjectProperties p) { properties = p;}

		//! Set which controller we monitor
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		//! Return which controller we monitor
		uint GetControllerNum() const {return controller_num;};
		
		void SetDebugFlag(bool d) {debug_flag = d;};
		bool GetDebugFlag() const {return debug_flag;};
		
		//! Handle collisions with another object
		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
		
		// When an animation we're playing loops, we get this call
		virtual void OnAnimationLooped() {};

		inline bool IsDead() const {return is_dead;};
		inline void SetIsDead(bool bVal) {is_dead = bVal;}

		ObjectLayer* const GetLayer() const {return m_pkLayer;};
		void SetLayer(ObjectLayer* const l) {m_pkLayer = l;};

		void SetObjectDefName(const char*);

		std::string GetObjectDefName();

		void ApplyImpulse(float x, float y);
		void ApplyImpulse(const b2Vec2& v);

		bool FinishLoading();
		
		virtual ~Object();

		//! True if the point 'p' is contained in the layer-space coordinates for this object
		//! (layer-space = object's bounding box adjusted for layer scroll speeds)
		bool ContainsPoint(const b2Vec2& p) const;

		virtual void ResetVolatileState(VolatileStateLevel level);

		static Object* AddPrototype(std::string type, Object* obj);
		static Object* CreateObject(std::string type);

		friend class ObjectFactory;
		friend class Editor;
};

#if !defined(SWIG) 
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Object)
BOOST_CLASS_VERSION(Object, 3)
BOOST_CLASS_VERSION(ObjectProperties, 4)
#endif // SWIG

#endif // __OBJECT_H