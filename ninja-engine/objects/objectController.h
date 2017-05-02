#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include "object.h"

class Sprite;

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
		ar & BOOST_SERIALIZATION_NVP(controller_num);
	}

	protected:
		Sprite* controller_sprite;
		vector<struct Button> buttons;

		bool only_show_during_demo;

		int controller_num;

		virtual bool LoadObjectProperties(XMLNode & xDef);
		
	public:
		IMPLEMENT_CLONE(ObjectController)

		bool Init();
		void Shutdown();

		void Clear();

		void Update();
		void Draw();

		virtual int GetWidth() const;
		virtual int GetHeight() const;

		ObjectController();
		~ObjectController();
};

#if !defined(SWIG) 
BOOST_CLASS_VERSION(ObjectController, 1)
#endif // SWIG

#endif // OBJ_CONTROLLER_H
