#ifndef FORCE_H
#define FORCE_H

#include "stl_wrapper.h"

class Force;
class GameState;
class Object;

typedef std::vector<Force*> ForceList;

#include "gameBase.h"
#include "vector2D.h"

//! All the different types of forces.
//! \bug Some of these we don't need.
enum ForceType {
	FORCE_GRAVITY,
	FORCE_INPUT,
	FORCE_INPUT1,	
	FORCE_INPUT2,	
	FORCE_SIMPLE_CIRCLE,
	FORCE_FRICTION
};

//! A 'force', part of a physics simulation. It generates an acceleration.

//! A newtonian force that acts on an object.  It computes a vector representing
//! the acceleration of a given phenomena (gravity, input, etc) on an object.
class Force : public GameBase {
	protected:
		ForceType type;
				
	public:
		bool Init(GameState* _game_state);
		void Shutdown();
		
		//! Compute the force vector that is created
		virtual Vector2D GetForce(Object* obj) = 0;
		
		//! Get the type of force this instance is
		inline ForceType GetType() {return type;};

		Force();
		virtual ~Force();
};


#endif // FORCE_H
