#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

// TODO: Rename this file to gameWorld.h

#include "stdafx.h"
#include "gameMode.h"

class Force;
class Object;
class ObjectFactory;
class ForceFactory;
class ObjectLayer;
			
// note: list is STL's doubly linked list
typedef list<Object*> ObjectList;
typedef list<Object*>::iterator ObjectListIter;
typedef list<Object*>::const_iterator ObjectConstListIter;
typedef list<Object*>::reverse_iterator ObjectListReverseIter;
typedef list<Object*>::const_reverse_iterator ObjectListConstReverseIter;

typedef vector<Object*> ObjectArray;
typedef vector<Object*>::iterator ObjectArrayIter;

//! Represents a physical simulation (the main game levels)
class GameWorld : public GameMode {

		DECLARE_SINGLETON_CLASS(GameWorld)

		protected:
			//! Filename of music, or NULL if none
			const char* music_file;

			//! Background color (use makecol())
			int bg_color;
			int bg_color_top; // if present, use both for a gradient

			//! ALL objects in the scene
			ObjectList m_objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> layers;

			//! Collection of forces
			vector<Force*> forces;

			//! Force factory: TODO singleton
			ForceFactory* forceFactory;
		
			//! List of objects to add on next Update()
			ObjectList objectAddList;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int width, height;

			//! Current camera XY position
			int camera_x, camera_y;

			//! Whether the camera is currently shaking or not
			bool is_camera_shaking;

			int camera_shake_time_total;
			int camera_shake_time;;

			//! Which object the camera should follow
			Object* camera_follow;

			//! Camera threshold - how far it should slide before snapping
			// you can use this to make sure we're, say, 60 units from the sides
			// at all times.
			int camera_side_margins;

			//! Camera snap rate - how fast the camera should "snap" to new targets
			float camera_snap_rate;

			//! How much to scale the X coordinate of the camera.
			//! MOSTLY used for scrolling backgrounds at different speeds
			//! on different layers
			float camera_scroll_speed;

			//! Physics functions
			void Solve(Object* obj);
			void CheckForCollisions(ObjectArray &collideableObjects, Object* obj);
			void GetCollideableObjects(ObjectArray &objs);
			bool CleanupObject(ObjectListIter &obj);

			//! Game update functions
			void UpdateObjects();

			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class, or something.
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int LoadObjectFromXML(XMLNode&,	XMLNode&, ObjectLayer* const);
			int LoadForcesFromXML(XMLNode&);
			int LoadLayerFromXML(XMLNode&, ObjectLayer* const);
			// these virtuals might be overridden by the map editor
			virtual int LoadObjectDefsFromXML(XMLNode&);
			virtual void LoadMusic(const char* filename);

			int CreateObjectFromXML(XMLNode &xObject, ObjectLayer* const);

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

			bool is_loading;
			bool use_scroll_speed;

			int camera_shake_x;
			int camera_shake_y;
			int camera_shake_fade_time_left;
				
			bool allow_player_offscreen;
	
			bool m_bJumpedBackFromADoor;

		public:
			virtual int Init(XMLNode);
			virtual void Shutdown();

			bool UseScrollSpeed() {return use_scroll_speed;}

			//! True if we are in the middle of the initial load
			inline bool IsLoading() {return is_loading;}

			//! Returns true during loading if we jumped back from a door
			bool JumpedBackFromADoor() const {return m_bJumpedBackFromADoor;}

			void SetModalObject(Object* obj) {modal_active = obj;};

			//! Add an object to the world
			// if addImmediately is false, this goes on the objectAddList
			// if addImmediately is true, this goes directly on the object list
			// NOTE you CANNOT directly add objects to the world during Update()'s
			void AddObject(	Object* obj, bool addImmediately = false);

			//! Delete an object from it's layer
			void DeleteObjectFromLayer(Object* obj);

			//! Find a layer by name
			ObjectLayer* FindLayer(const char* name);
			
			virtual void Draw();
			virtual void Update();

			void DoMainGameUpdate();

			int GetWidth() {return width;};
			int GetHeight() {return height;};

			void ComputeNewCamera();
			void SetCameraScrollSpeed(float s) {camera_scroll_speed = s;};

			int GetCameraX(); 
			int GetCameraY();

			void SetCameraShake(bool state, int fade_out_time = -1);
			
			void TransformWorldToView(int &x, int &y);
			void TransformViewToScreen(int &x, int &y);

			void ShowText(	const char* txt, 
											const char* avatar_filename = 0, 
											bool modal_active = false);

			//! Make the camera snap IMMEDIATELY to its
			//! target's position rather than doing the nice
			//! floaty thing
			void SnapCamera();
			
			//! Experimental: Get AI fitness score for AI traning
			int GetAiFitnessScore();

			//! Allows the player to remain offscreen
			//! Only use for cinematics.
			inline void AllowPlayerOffscreen(bool state) {
				allow_player_offscreen = state;
			}

			bool PlayerAllowedOffscreen() {
				return allow_player_offscreen;
			}


			virtual ~GameWorld();

			friend class MapSaver;
};

#define WORLD (GameWorld::GetInstance())

#endif
