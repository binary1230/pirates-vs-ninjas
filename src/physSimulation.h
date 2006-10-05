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
			
// note: list is STL's doubly linked list
typedef list<Object*> ObjectList;
typedef list<Object*>::iterator ObjectListIter;
typedef list<Object*>::reverse_iterator ObjectListReverseIter;

//! Represents a physical simulation (the main game levels)
class PhysSimulation : public GameMode {
		protected:		
			//! ALL objects in the scene
			ObjectList objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> layers;

			//! Collection of forces
			vector<Force*> forces;

			//! Force factory: TODO singleton
			ForceFactory* forceFactory;
		
			//! Temporary. Belongs in GameMusic
			OGGFILE* music;

			//! List of objects to add on next Update()
			ObjectList objectAddList;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int width, height;

			//! Current camera XY position
			int camera_x, camera_y;

			//! Which object the camera should follow
			Object* camera_follow;

			//! How much to scale the X coordinate of the camera.
			//! MOSTLY used for scrolling backgrounds at different speeds
			//! on different layers
			float camera_scroll_speed;

			//! Physics functions
			void Solve(Object* obj);
			void CheckForCollisions(ObjectList &collideableObjects, 
															Object* obj);
			void GetCollideableObjects(ObjectList &objs);
			bool CleanupObject(ObjectListIter &obj);

			//! Game update functions
			void UpdateObjects();

			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class, or something.
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int LoadObjectFromXML(XMLNode&,	XMLNode&, ObjectLayer*);
			int LoadForcesFromXML(XMLNode&);
			int LoadLayerFromXML(XMLNode&, ObjectLayer*);
			int CreateObjectFromXML(XMLNode &xObject, ObjectLayer *layer);

			//! Check and see if an object is dead and needs to be cleaned up
			bool CheckIsDead(Object* obj);

			//! ONLY used during init, temp variables for "repeat" xml tags
			int repeater_current_x, repeater_current_y;

			//! Modal object
			//! If a modal object (e.g. on-screen text) is active
			//! then the rest of the game pauses until it responds
			Object* modal_active;
			
			//! Do the real work of adding an object to the global object list
			void DoAddObject(Object* obj);
		
			void DoCleaning();

		public:
			int Init(XMLNode);
			void Shutdown();

			void SetModalObject(Object* obj) {modal_active = obj;};

			//! Add an object to the world
			// if addImmediately is false, this goes on the objectAddList
			// if addImmediately is true, this goes directly on the object list
			// NOTE you CANNOT directly add objects to the world during Update()'s
			void AddObject(	Object* obj, bool addImmediately = false);
			
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
			
			//! Experimental: Get AI fitness score for AI traning
			int GetAiFitnessScore();

			PhysSimulation();
			~PhysSimulation();
};

#endif
