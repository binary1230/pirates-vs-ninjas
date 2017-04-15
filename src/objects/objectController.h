#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include "object.h"

class Sprite;
class ObjectFactory;

//! A button on the Controller and its associated sprite
struct Button {
	bool active;
	Sprite* sprite;
};

//! A drawable on-screen joystick Object that is shown during demo playback
class ObjectController : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:
		Sprite* controller_sprite;
		vector<struct Button> buttons;

		bool only_show_during_demo;

		virtual bool LoadObjectProperties(XMLNode & xDef);
		
	public:
		bool Init();
		void Shutdown();

		void Clear();

		void Update();
		void Draw();

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ObjectController();
		~ObjectController();

		friend class ObjectFactory;
};

#endif // OBJ_CONTROLLER_H
