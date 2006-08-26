#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include <allegro.h>
#include <vector>
using namespace std;

class GameState;
class Sprite;
class ObjectFactory;
class PhysSimulation;

#include "object.h"
#include "xmlParser.h"

//! A button on the Controller and its associated sprite
struct Button {
	bool active;
	Sprite* sprite;
};

//! A drawable on-screen joystick Object that is shown during demo playback
class ControllerObject : public Object {
	protected:
		Sprite* controller_sprite;
		vector<struct Button> buttons;

		bool only_show_during_demo;
		
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);
		
		void Shutdown();

		void Update();
		void Draw();

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ControllerObject();
		~ControllerObject();

		friend class ObjectFactory;
};

#endif // OBJ_CONTROLLER_H
