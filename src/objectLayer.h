#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

#include "stdafx.h"

class Object;

//! A container class representing "layers" (e.g. background, foreground, etc)

//! A layer holds pointers to the objects on it. Its main purpose is to 
//! allow Object's to be drawn in the correct order on-screen and allow
//! multiple scrolling speeds
//
// TODO The layers holding pointers to objects is a bit complex and probably
//      needs rethinking
class ObjectLayer {
	protected:
		list<Object*> objects;
		bool visible;
		float scroll_speed;

		// the following are rarely used so are stored as pointers
		// XML props
		CString* name;
	
	public:
		bool Init();
		void Shutdown();

		void SetName(const char*);

		//! Get/set layer scroll speed
		void SetScrollSpeed(float _scroll_speed) { scroll_speed = _scroll_speed ;};
		float GetScrollSpeed() { return scroll_speed; };

		//! Get/Set layer visibility
		bool IsVisible() { return visible; };
		void SetVisible(bool _visible) { visible = _visible; };

		//! Draw this layer with camera scroll take into effect
		void Draw();
		
		//! Put an object onto this layer
		void AddObject(Object*);

		//! Remove an object from this layer
		void RemoveObject(Object*);
		
		ObjectLayer();
		virtual ~ObjectLayer();

		friend class MapSaver;
};

#endif // OBJECTLAYER_H
