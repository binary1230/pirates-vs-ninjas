#ifndef DOOROBJECT_H
#define DOOROBJECT_H

#include "stdafx.h"

#include "object.h"

class ObjectFactory;
class PhysSimulation;

enum DoorType {
	INVALID_TYPE = -1,

	SWITCH_TO_ANOTHER_MODE = 0,
	LEVEL_EXIT,
	WARP_TO_ANOTHER_PORTAL,
	RETURN_TO_LAST_MODE
};

class DoorObject : public Object {
	protected:
		int door_open_time;
		
		CString door_name;		// the door name, used for when we switch levels 
													// around so we can jump back to it if needed

		CString mode_to_jump_to_on_activate;		// which mode we should use when this door
																						// is activated (e.g. "level2")
	
		enum DoorType door_type; 
	
		void DoDoorAction();
	
	public:
		bool Init(PhysSimulation *p);
		void Shutdown();
		
		void Update();

		// Activate this door.  when it is finished its animation,
		// the door's action will happen (e.g. warp to next level)
		void Activate();

		DoorObject();
		~DoorObject();

		inline const CString& GetName() {return door_name;}
			
		friend class ObjectFactory;
};

#endif // DoorObject_H   
