#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "objectLayer.h"
#include "gameMode.h"
#include "object.h"

class Object;
class ObjectFactory;
class ObjectLayer;
class PlayerObject;
			
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
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar  & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GameMode);
			ar  & boost::serialization::make_nvp("layers", m_kLayers);
			ar  & boost::serialization::make_nvp("bgcolor", m_bgColor);
			ar  & boost::serialization::make_nvp("bgcolor_top", m_bgColor);
			ar  & boost::serialization::make_nvp("music", m_szMusicFile);
			ar  & boost::serialization::make_nvp("width", m_iLevelWidth);
			ar  & boost::serialization::make_nvp("height", m_iLevelHeight);
			ar  & boost::serialization::make_nvp("lua_script", m_szLuaScript);
		}

		protected:
			//! Filename of music, or NULL if none
			std::string m_szMusicFile;

			//! Background color (use al_map_rgb())
			ALLEGRO_COLOR m_bgColor;
			ALLEGRO_COLOR m_bgColorTop; // if present, use both for a gradient, else -1

			//! ALL objects in the scene
			ObjectList m_objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> m_kLayers;
		
			//! List of objects to add on next Update()
			ObjectList m_kObjectsToAdd;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int m_iLevelWidth, m_iLevelHeight;

			//! Current camera XY position
			int m_iCameraX, m_iCameraY;

			//! Whether the camera is currently shaking or not
			bool m_bIsCameraShaking;

			int m_iCameraTotalShakeTime;
			int m_iCameraShakeTime;

			//! Which object the camera should follow
			Object* m_pkCameraLookatTarget;

			//! Camera threshold - how far it should slide before snapping
			// you can use this to make sure we're, say, 60 units from the sides
			// at all times.
			int m_iCameraSideMargins;

			//! Camera snap rate - how fast the camera should "snap" to new targets
			float m_fCameraSnapRate;

			//! How much to scale the X coordinate of the camera.
			//! MOSTLY used for scrolling backgrounds at different speeds
			//! on different layers
			float m_fCameraScrollSpeed;

			// Stuff saved for map editor:
			XMLNode m_xEffects;

			//! Game update functions
			void UpdateObjects();

			void AddNewObjectsIfNeeded();

			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class, or something.
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int LoadObjectFromXML(XMLNode&,	XMLNode&, ObjectLayer* const);
			int LoadLayerFromXML(XMLNode&, ObjectLayer* const);
			// these virtuals might be overridden by the map editor
			virtual int LoadObjectDefsFromXML(XMLNode&);
			virtual void LoadMusic(const char* filename);

			int CreateObjectFromXML(XMLNode &xObject, ObjectLayer* const);

			void CachePlayerObjects();

			//! ONLY used during init, temp variables for "repeat" xml tags
			int repeater_current_x, repeater_current_y;

			//! Modal object
			//! If a modal object (e.g. on-screen text) is active
			//! then the rest of the game pauses until it responds
			Object* modal_active;
			
			//! Do the real work of adding an object to the global object list
			void DoAddObject(Object* obj);
		
			void RemoveDeadObjectsIfNeeded();

			bool is_loading;
			bool use_scroll_speed;

			int camera_shake_x;
			int camera_shake_y;
			int camera_shake_fade_time_left;
				
			bool allow_player_offscreen;
	
			bool m_bJumpedBackFromADoor;

			std::string m_szLuaScript;

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

			//! Find a layer by name
			ObjectLayer* FindLayer(const char* name);

			PlayerObject* GetPlayer(uint iIndex)
			{
				assert(iIndex >= 0 && iIndex < m_kCachedPlayers.size());
				if (iIndex >= 0 && iIndex < m_kCachedPlayers.size())
					return m_kCachedPlayers[iIndex];
				else
					return NULL;
			}

			uint GetNumPlayers() { return m_kCachedPlayers.size(); }

			vector<PlayerObject*> m_kCachedPlayers;
			
			virtual void Draw();
			virtual void Update();

			void DoMainGameUpdate();

			int GetWidth() {return m_iLevelWidth;};
			int GetHeight() {return m_iLevelHeight;};

			void ComputeNewCamera();
			void SetCameraScrollSpeed(float s) {m_fCameraScrollSpeed = s;};

			int GetCameraX(); 
			int GetCameraY();

			void SetCameraShake(bool state, int fade_out_time = -1);
			
			void TransformWorldToView(int &x, int &y);
			void TransformViewToScreen(int &x, int &y);

			void ShowText(	const char* txt, 
							const char* avatar_filename = 0, 
							bool modal_active = false );

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

			void SaveMap();

			virtual ~GameWorld();
};

#define WORLD (GameWorld::GetInstance())

#endif // GAMEWORLD_H
