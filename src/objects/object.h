#ifndef OBJECT_H
#define OBJECT_H

#include "globals.h"

#include "rect.h"
#include "animations.h"

class Object;
class Animation;
class Sprite;
class ObjectLayer;
class ObjectFactory;
class b2Body;

struct CollisionDirection {
	unsigned up : 1;
	unsigned down : 1;
	unsigned left : 1;
	unsigned right : 1;
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

	//! If physical, another physical object cannot move through it
	bool is_physical;

	//! If static, this object WILL NOT MOVE, ever.
	//! Only matters if is_physical is on
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

	//! Object "types" (really should use RTTI or something.)
	bool is_player;
	bool is_spring;
	bool is_collectable;
	bool is_fan;
	bool is_door;
	bool is_ring;
	bool is_ball;

	bool is_badguy;
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
			ar & BOOST_SERIALIZATION_NVP(p.is_physical);
			ar & BOOST_SERIALIZATION_NVP(p.is_static);
			ar & BOOST_SERIALIZATION_NVP(p.is_sensor);
			ar & BOOST_SERIALIZATION_NVP(p.ignores_physics_rotation);
			ar & BOOST_SERIALIZATION_NVP(p.do_our_own_rotation);
			ar & BOOST_SERIALIZATION_NVP(p.use_angled_corners_collision_box);
			ar & BOOST_SERIALIZATION_NVP(p.is_overlay);

			ar & BOOST_SERIALIZATION_NVP(p.is_player);
			ar & BOOST_SERIALIZATION_NVP(p.is_spring);
			ar & BOOST_SERIALIZATION_NVP(p.is_collectable);
			ar & BOOST_SERIALIZATION_NVP(p.is_fan);
			ar & BOOST_SERIALIZATION_NVP(p.is_door);
			ar & BOOST_SERIALIZATION_NVP(p.is_ring);
			ar & BOOST_SERIALIZATION_NVP(p.is_ball);

			ar & BOOST_SERIALIZATION_NVP(p.is_badguy);
		}
	}
}

//! Clears property masks
inline void ClearProperties(struct ObjectProperties& p) {
	p.feels_gravity = 0;
	p.feels_user_input = 0;
	p.feels_friction = 0;
	p.is_overlay = 0;
	p.is_physical = 0;
	p.use_angled_corners_collision_box = 0;
	p.is_player = 0;
	p.is_spring = 0;
	p.is_collectable = 0;
	p.is_fan = 0;
	p.is_door = 0;
	p.is_ring = 0;
	p.is_ball = 0;
	p.spawns_enemies = 0;
	p.is_badguy = 0;
	p.is_static = 0;
	p.is_sensor = 0;
	p.ignores_physics_rotation = 0;
	p.do_our_own_rotation = 0;
}

// Used for find()
bool ObjectIsDead(Object* obj);

//! A drawable entity in the world

//! Objects have physical properties associated with them, but do
//! not always have to take part in the world
class Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */)
	{
		ar & BOOST_SERIALIZATION_NVP(pos);
		ar & BOOST_SERIALIZATION_NVP(objectDefName);
		ar & BOOST_SERIALIZATION_NVP(controller_num);
		ar & BOOST_SERIALIZATION_NVP(properties);
	}

	protected:

		//! A pointer to the layer this object is on
		ObjectLayer* m_pkLayer;

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;

		//! CACHED level width and height
		int level_width, level_height;
		
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

		// XML Props, rarely ever used.
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

		//! Update display times
		void UpdateDisplayTime();

		//! (optional) Do a simple update of the animations
		//! Most objects will call this
		void UpdateSimpleAnimations();

		//! Update the fading stuff
		void UpdateFade();

		//! Width and Height of the object
		// (we may need to rethink where these come from)
		int width, height;

		//! Bounding box offsets from the bottom left of the first sprite
		// (maye need to play with these)
		int b_box_offset_x, b_box_offset_y;

		//! Rotational parameters
		float rotate_angle, rotate_velocity;
		bool use_rotation;

		//! Whether to draw the bounding box or not
		bool m_bDrawBoundingBox;

		//! If this object should report collisions or not
		bool m_bCanCollide;
	
	public:
		// WRONG Protected constructor, this means we can't directly
		// instantiate Object's, we need to use a friend or derived class.
		Object();

		int tmp_debug_flag;

		// Whether to draw ALL the different rectangles or not (DEBUG)
		static bool debug_draw_bounding_boxes;
		
		// DEBUG ONLY: A unqiue ID that is incremented every time an object
		// is created.  The amount of created objects should match the amount
		// of free'd objects.
		static unsigned long debug_object_id;
		unsigned long unique_id;

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update() = 0;

		virtual void InitPhysics();

		// TODO: Make this take an animation code, for now it just takes the index of the animation 
		// as defined by the order we found them in the XML file.  Very prone to errors. HACKY
		virtual void PlayAnimation(uint uiIndex);

		void SetDrawBounds(bool bDrawBounds) {m_bDrawBoundingBox = bDrawBounds;}
		
		//! Fade this object out over a given time (in frames)
		void FadeOut(int time);

		virtual void Draw();

		void Transform(	int &x, int &y, 
										const int &offset_x = 0, const int &offset_y = 0);

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
		inline int GetX() const				{ return (int)pos.x; }
		inline int GetY() const				{ return (int)pos.y; }
		inline b2Vec2 GetXY() const { return pos; }; 

		inline void SetX(const int _x) 		{ pos.x = _x; }
		inline void SetY(const int _y) 		{ pos.y = _y; }
		inline void SetXY(const int _x, const int _y) {
				pos.x = _x;
				pos.y = _y;
		}
		inline void SetXY(const b2Vec2 &_pos) {
			pos = _pos;
		}

		inline int GetAlpha() { return alpha; };
		inline void SetAlpha(const int a) { alpha = a; };

		inline void SetFlipX(const bool val) { flip_x = val; };
		inline void SetFlipY(const bool val) { flip_y = val; };

		//! Functions to get/set velocity
		inline float GetVelX() 					{ 
			return m_pkPhysicsBody ? m_pkPhysicsBody->GetLinearVelocity().x : 0.0f; 
		}

		inline float GetVelY() 					{ 
			return m_pkPhysicsBody ? m_pkPhysicsBody->GetLinearVelocity().y : 0.0f; 
		}

		inline b2Vec2 GetVelXY() const { 
			return m_pkPhysicsBody ? m_pkPhysicsBody->GetLinearVelocity() : b2Vec2(0.0f, 0.0f); 
		}

		inline void SetVelX(const float _vx) 		{ 
			if (m_pkPhysicsBody)
				m_pkPhysicsBody->SetLinearVelocity(b2Vec2(_vx, m_pkPhysicsBody->GetLinearVelocity().y));
		}
		inline void SetVelY(const float _vy) 		{ 
			if (m_pkPhysicsBody)
				m_pkPhysicsBody->SetLinearVelocity(b2Vec2(m_pkPhysicsBody->GetLinearVelocity().x, _vy));
		}

		inline void SetVelXY(const float _vx, const float _vy) {
			if (m_pkPhysicsBody)
				m_pkPhysicsBody->SetLinearVelocity(b2Vec2(_vx, _vy));
		}

		inline void SetVelXY(const b2Vec2& v) {
			if (m_pkPhysicsBody)
				m_pkPhysicsBody->SetLinearVelocity(v); 
		}
		
		inline void SetVelRotate(const float vel) {
			rotate_velocity = vel;
		}

		inline void SetUseRotation(const bool state) {
			use_rotation = state;
		}

		//! Get width/height of this object
		inline int GetWidth() const {return width;};
		inline int GetHeight() const {return height;};
	
		//! Physics: reset this object's physics stuff for next frame
		void ResetForNextFrame();
		
		struct ObjectProperties GetProperties() const { return properties; };
		inline void SetProperties(struct ObjectProperties p) { properties = p;}

		//! Setup some commonly used variables
		void SetupCachedVariables();

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
		
		//! Returns true if this type of object is able to collide with another
		inline bool CanCollide() const {
			return m_bCanCollide;
		}

		void SetObjectDefName(const char*);

		void ApplyImpulse(float x, float y);
		void ApplyImpulse(const b2Vec2& v);
		
		virtual ~Object();

		b2Body* m_pkPhysicsBody;

		friend class ObjectFactory;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Object)

#endif // OBJECT_H