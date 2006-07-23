#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

#include <list>
#include <vector>
#include <map>
using namespace std;

class PhysSimulation;
class GameState;
class Force;
class Object;
class ObjectFactory;
class ForceFactory;
class ObjectLayer;

#include "gameBase.h"
#include "gameMode.h"
#include "StdString.h"
#include "xmlParser.h"

//! Maps an object definition name to an XMLNode 
//! (e.g. maps "bad_guy_1" to its corresponding XML data)
//! only used for parsing XML
typedef map<const CString, XMLNode> ObjectDefMapping;

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
			void UpdateObjects();
		
			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int PhysSimulation::LoadObjectFromXML(XMLNode&,	XMLNode&, ObjectLayer*);
			int LoadForcesFromXML(XMLNode&);
			int LoadObjectDefsFromXML(XMLNode&, ObjectDefMapping&);
			int LoadLayerFromXML(XMLNode &, ObjectLayer*, ObjectDefMapping&);

		public:
			int Init(GameState*, XMLNode);
			void Shutdown();

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

			PhysSimulation();
			~PhysSimulation();
};

#endif
