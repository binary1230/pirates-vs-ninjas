#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

#include "stdafx.h"
#include "singleton.h"
#include "gameMode.h"

class PhysSimulation;
class Force;
class Object;
class ObjectFactory;
class ForceFactory;
class ObjectLayer;
class OGGFILE;

typedef list<Object*> ObjectList;
typedef list<Object*>::iterator ObjectListIter;
typedef list<Object*>::reverse_iterator ObjectListReverseIter;

//! Represents a physical simulation (the main game levels)
class PhysSimulation : public GameMode {
		protected:		
			//! ALL objects in the scene
			// note: list is STL's doubly linked list
			list<Object*> objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> layers;

			//! Collection of forces
			vector<Force*> forces;
		
			//! Creates new objects
			ObjectFactory *objectFactory;

			//! Creates new forces
			ForceFactory *forceFactory;

			//! Temporary. Belongs in GameMusic
			OGGFILE* music;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int width, height;

			int camera_x, camera_y;

			//! Which object the camera should follow
			Object* camera_follow;

			//! How much to scale the X coordinate of the camera.
			//! MOSTLY used for scrolling backgrounds at different speeds
			//! on different layers
			float camera_scroll_speed;

			//! Physics functions
			void ResetForNextFrame();
			void Solve();

			//! Game update functions
			void UpdateObjects();

			//! Update an object, return false if we need to delete it
			bool UpdateObject(Object* obj);
		
			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class, or something.
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int LoadObjectFromXML(XMLNode&,	XMLNode&, ObjectLayer*);
			int LoadForcesFromXML(XMLNode&);
			int LoadLayerFromXML(XMLNode&, ObjectLayer*);
			int CreateObjectFromXML(XMLNode &xObject, ObjectLayer *layer);

			//! ONLY used during init, temp variables for "repeat" xml tags
			int repeater_current_x, repeater_current_y;

		public:
			int Init(XMLNode);
			void Shutdown();
			
			void AddObject(Object* obj, ObjectLayer* layer);

			void Draw();
			void Update();

			int GetWidth() {return width;};
			int GetHeight() {return height;};

			void ComputeNewCamera();
			void SetCameraScrollSpeed(float s) {camera_scroll_speed = s;};

			int GetCameraX() {return camera_x;};
			int GetCameraY() {return camera_y;};
			
			void TransformWorldToView(int &x, int &y);
			void TransformViewToScreen(int &x, int &y);

			void MoveObjectsToNewPositions();
			void CheckForCollisions();
			void GetCollideableObjects(vector<Object*> &objs);
				
			//! Experimental: Get AI fitness score for AI traning
			int GetAiFitnessScore();

			PhysSimulation();
			~PhysSimulation();
};

#endif
