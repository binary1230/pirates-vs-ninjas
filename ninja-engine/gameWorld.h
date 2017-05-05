#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "objectLayer.h"
#include "gameMode.h"
#include "object.h"
#include "globals.h"

class Object;
class ObjectLayer;
class ObjectPlayer;
class Editor;
class Camera;
class CameraFollow;
			
typedef list<Object*> ObjectList;
typedef vector<Object*> ObjectArray;

//! Represents a physical simulation (the main game levels)
class GameWorld : public GameMode {

		DECLARE_SINGLETON_CLASS(GameWorld)
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GameMode);
			ar & boost::serialization::make_nvp("objects", _objects);
			ar & boost::serialization::make_nvp("layers", _layers);
			ar & boost::serialization::make_nvp("bgcolor", _bgColor);
			ar & boost::serialization::make_nvp("bgcolor_top", _bgColorTop);
			ar & boost::serialization::make_nvp("music", _musicFile);
			ar & boost::serialization::make_nvp("width", _levelWidth);
			ar & boost::serialization::make_nvp("height", _levelHeight);
			ar & boost::serialization::make_nvp("lua_script", _luaScript);
			ar & BOOST_SERIALIZATION_NVP(_camera);
			ar & BOOST_SERIALIZATION_NVP(m_included_effect_xml_files);
			ar & BOOST_SERIALIZATION_NVP(m_included_objectdef_xml_files);
		}

		protected:
			Editor* map_editor;

			//! Filename of music, or NULL if none
			std::string _musicFile;

			//! Background color (use al_map_rgb())
			ALLEGRO_COLOR _bgColor;
			ALLEGRO_COLOR _bgColorTop; // if present, use both for a gradient, else -1

			//! ALL objects in the scene
			ObjectList _objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> _layers;

			vector<std::string> m_included_effect_xml_files;
			vector<std::string> m_included_objectdef_xml_files;
		
			//! List of objects to add on next Update()
			ObjectList _objectsToAdd;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int _levelWidth, _levelHeight;

			bool is_loading;
			bool use_scroll_speed;

			bool allow_player_offscreen;

			bool m_bJumpedBackFromADoor;

			std::string _luaScript;

			//! Game update functions
			void UpdateObjects();

			void AddNewObjectsIfNeeded();

			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class, or something.
			int Load();
			bool FinishLoadingObjects();
		
			// these virtuals might be overridden by the map editor
			virtual void LoadMusic(const char* filename);
			virtual bool LoadObjectDefsFromXML();

			bool InitJumpBackFromDoor();

			void CachePlayerObjects();

			//! ONLY used during init, temp variables for "repeat" xml tags
			int repeater_current_x, repeater_current_y;

			//! Modal object
			//! If a modal object (e.g. on-screen text) is active
			//! then the rest of the game pauses until it responds
			Object* modal_active;

			vector<ObjectPlayer*> m_kCachedPlayers;
		
			void RemoveDeadObjectsIfNeeded();

			void Clear();

			CREATE_PROPERTY(bool, AllowExiting)

			Camera* _camera;
			Object* m_pkCameraLookatTarget; // temp, will go away once we're doing serializing correctly.

		public:
			// not a very effecient method.  call with map editor only
			inline vector<Object*> GetObjects() {
				return vector<Object*>{ std::begin(_objects), std::end(_objects) };
			}

			virtual int Init(XMLNode xMode);
			virtual void Shutdown();

			inline Editor* GetEditor() {
				return map_editor;
			}

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

			inline vector<ObjectLayer*> GetLayers() {
				return _layers;
			}

			ObjectPlayer* GetPlayer(uint iIndex)
			{
				assert(iIndex >= 0 && iIndex < m_kCachedPlayers.size());
				if (iIndex >= 0 && iIndex < m_kCachedPlayers.size())
					return m_kCachedPlayers[iIndex];
				else
					return NULL;
			}

			uint GetNumPlayers() { return m_kCachedPlayers.size(); }
			
			virtual void Draw();
			virtual void Update();

			void DoMainGameUpdate();

			void SaveWorldOverCurrentFile();

			static GameWorld* CreateWorld(string mode_filename);

			int GetWidth() {return _levelWidth;};
			int GetHeight() {return _levelHeight;};

			void SetCameraShake(bool state, int fade_out_time = -1);
			
			void TransformWorldToView(float & x, float & y, float scroll_speed_multiplier);
			void TransformViewToScreen(float & x, float & y);

			CameraFollow* GetPlayerCamera();

			void ShowText(	const char* txt, 
							const char* avatar_filename = 0, 
							bool modal_active = false );

			Object * FindObjectByID(unsigned long id);

			inline Camera* GetCamera() { return _camera; }

			//! Allows the player to remain offscreen
			//! Only use for cinematics.
			inline void AllowPlayerOffscreen(bool state) {
				allow_player_offscreen = state;
			}

			bool PlayerAllowedOffscreen() {
				return allow_player_offscreen;
			}

			void SaveWorld(string filename = "test-save.xml");

			virtual ~GameWorld();

			friend class Editor;
};

#define WORLD (GameWorld::GetInstance())

#endif // GAMEWORLD_H
