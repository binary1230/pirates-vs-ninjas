// \todo Free all animations that are loaded.
#ifndef OBJECT_H
#define OBJECT_H

class Object;
class Force;
class Animation;
class PhysSimulation;
class Sprite;
class ObjectLayer;

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

#include "globals.h"
#include "gameBase.h"
#include "rect.h"
#include "vector2D.h"
#include "xmlParser.h"
#include "animations.h"

struct CollisionDirection {
	unsigned up : 1;
	unsigned down : 1;
	unsigned left : 1;
	unsigned right : 1;
};

//! Various properties of an Object
struct ObjectProperties {
	
	// NOTE: If you add anything here, update ClearProperties()
	bool feels_gravity;	
	bool feels_user_input;
	bool feels_friction;

	//! If solid, another solid object cannot move through it
	bool is_solid;
	bool is_player;
	bool is_spring;
	
	//! true if this object is an overlay
	//! e.g. not IN the world, but on top it,
	//! like our status bar or health or something.
	bool is_overlay;

	int spring_strength;
};

//! Clears property masks
inline void ClearProperties(struct ObjectProperties& p) {
	p.feels_gravity = 0;
	p.feels_user_input = 0;
	p.feels_friction = 0;
	p.is_overlay = 0;
	p.is_solid = 0;
	p.is_player = 0;
	p.is_spring = 0;
}

//! A drawable entity in the physics simulation

//! Objects have physical properties associated with them, but do
//! not always have to take part in the physics simulation.
class Object : public GameBase {
	protected:

		//! A pointer to the layer this object is on
		ObjectLayer* layer;

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;
		
		//! Real Width/height of this object
		int width, height;

		//! Bounding box for this object
		Rect bbox;

		//! Projection rectange (for collisions)
		Rect projRect;

		//! CACHED level width and height
		int level_width, level_height;
		
		//! CACHED simulation this object is a part of
		PhysSimulation* simulation;
					
		//! Current position
		Vector2D pos;

		//! Old position (last frame)
		Vector2D old_pos;

		//! Current velocity 
		Vector2D vel;

		//! Current force
		Vector2D accel;
		
		//! The directions of current collisions (up,down,right,left)
		CollisionDirection d;
		
		//! Object properties
		struct ObjectProperties properties;
		
		//! XXX OLD Bitmap to draw when this object is drawn
		// BITMAP* bitmap;
		// bool bitmap_is_deleteable;

		//! Points to the current animation
		Animation* currentAnimation;

		//! Array of all loaded animations
		vector<Animation*> animations;

		//! Points to the sprite we should draw on next Draw()
		Sprite* currentSprite;

		//! Whether to flip the sprite when drawing
		bool flip_x;
		bool flip_y;

		//! Object's Physical Mass
		float mass;

		//! Solve for the new position of this object
		Vector2D Solve();


		//! Base class initialization
		bool BaseInit();	

		//! True if this object is no longer in play and needs to be cleaned up
		bool is_dead;

		//! If true, this object can print debug info out if it wants to.
		bool debug_flag;
		
	public:
		virtual bool Init(GameState* _game_state) = 0;
		virtual void Shutdown();
		
		virtual void Update() = 0;

		//! Move this object to its new position (done before 
		//! collision detection)
		void MoveToNewPosition();

		//! Move this object back to its previous frame's position
		void MoveBack();

		virtual void Draw();

		void Transform(int &x, int &y, int offset_x = 0, int offset_y = 0);
		void TransformRect(Rect &r);
		
		//! Draw this object at its coordinates plus specified offset
		//! Optionally, you can pass in a specific sprite to draw, otherwise
		//! It'll just use the current sprite (most cases)
		void DrawAtOffset(int x, int y, Sprite* = NULL);	
		
		//! Functions to get/set position
		inline int GetX() 					{ return (int)pos.GetX(); }
		inline int GetY() 					{ return (int)pos.GetY(); }
		inline void SetX(int _x) 		{ pos.SetX((float)_x);  }
		inline void SetY(int _y) 		{ pos.SetY((float)_y);	}
		inline void SetXY(int _x, int _y) {
				pos.SetX((float)_x);	
				pos.SetY((float)_y);
		}

		inline void SetFlipX(bool val) { flip_x = val; };
		inline void SetFlipY(bool val) { flip_y = val; };

		//! Functions to get/set velocity
		inline float GetVelX() 					{ return vel.GetX(); }
		inline float GetVelY() 					{ return vel.GetY(); }
		inline void SetVelX(float _vx) 		{ vel.SetX(_vx); }
		inline void SetVelY(float _vy) 		{ vel.SetY(_vy);	}
		inline void SetVelXY(float _vx, float _vy) {
				vel.SetX(_vx);
				vel.SetY(_vy);
		}
		
		//! Get width/height of this object
		virtual int GetWidth(); 
		virtual int GetHeight(); 
	
		//! Physics: reset this object's physics stuff for next frame
		void ResetForNextFrame();

		//! Apply a force to this object
		void ApplyForce(Force* f);
		
		inline float GetMass() 					{ return mass; }
		inline void SetMass(float m) 		{ mass = m; }
		
		inline struct ObjectProperties GetProperties() { return properties; }
		inline void SetProperties(struct ObjectProperties p) { properties = p;}

		//! Setup some commonly used variables
		void SetupCachedVariables();

		//! Load animations from an XML object definition 
		bool LoadAnimations(XMLNode &xDef, AnimationMapping* = NULL);

		//! Loads all the sounds from an XML file
		bool LoadSounds(XMLNode &xDef);
		
		//! Set which controller we monitor
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		//! Return which controller we monitor
		uint GetControllerNum() {return controller_num;};
		
		void SetDebugFlag(bool d) {debug_flag = d;};
		bool GetDebugFlag() {return debug_flag;};
		
		bool LoadProperties(XMLNode &xDef);		//! Load object properties from XML
		
		//! Handle collisions with another object
		virtual void Collide(Object* obj);

		bool const IsColliding(Object *obj);

		inline bool IsDead() {return is_dead;};

		//! Returns a vector used for collision detection
		//! This vector will be have a position that is guaranteed
		//! to make the passed object NOT collide with THIS object.
		//! (based on velocity)
		CollisionDirection GetBound(Object* obj, Vector2D &v);

		void UpdateProjectionRect();
		Rect GetProjectionRect() {return projRect;}

		//! Plays a sound, or does nothing if that sound is not loaded
		void PlaySound(CString name);

		ObjectLayer* GetLayer() {return layer;};
		void SetLayer(ObjectLayer* l) {layer = l;};
		
		Object();
		virtual ~Object();
};

#endif // OBJECT_H
