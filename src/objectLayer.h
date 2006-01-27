#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

#include <allegro.h>
#include <vector>
using namespace std;

#include "gameBase.h"

class Object;

//! A container class representing "layers" (e.g. background, foreground, etc)

//! A layer holds the objects on it. Its main purpose is to allow Object's
//! to be drawn in the correct order on-screen
class ObjectLayer : public GameBase {
	protected:
		vector<Object*> objects;
		bool visible;
		float scroll_speed;
	
	public:
		bool Init(GameState*);
		void Shutdown();

		//! Get/set layer scroll speed
		void SetScrollSpeed(float _scroll_speed) { scroll_speed = _scroll_speed ;};
		float GetScrollSpeed() { return scroll_speed; };

		//! Get/Set layer visibility
		bool IsVisible() { return visible; };
		void SetVisible(bool _visible) { visible = _visible; };

		//! Draw this layer with camera scroll take into effect
		void Draw();
		
		//! Push an object onto this layer
		void PushObject(Object*);
		
		ObjectLayer();
		virtual ~ObjectLayer();
};

#endif // OBJECTLAYER_H
