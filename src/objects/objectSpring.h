#ifndef SPRING_OBJECT_H
#define SPRING_OBJECT_H

#include "object.h"

class ObjectFactory;

#define DEFAULT_SPRING_STRENGTH_X 0.0f
#define DEFAULT_SPRING_STRENGTH_Y 20.0f

//! A "simple" Object (e.g. scenery) - No collisions
class ObjectSpring : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		ar & boost::serialization::make_nvp("springDirection", spring_vector);
	}

	protected:
		uint spring_reset_time;
		bool spring_is_active;

		// When an object hits the spring, it's 
		// velocity will be set to this value
		b2Vec2 spring_vector;
				
	public:
		IMPLEMENT_CLONE(ObjectSpring)

		bool Init();
		virtual void Clear();
		void Shutdown();
		void Update();

		bool LoadSpringVectorFromXML(XMLNode & xSpringDirection);

		virtual bool LoadXMLInstanceProperties(XMLNode & xObj);

		virtual bool LoadObjectProperties(XMLNode & xDef);

		ObjectSpring();
		~ObjectSpring();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
		bool IsSpringActive() {return spring_is_active;};

		const b2Vec2& GetSpringVector() const {return spring_vector;};

		friend class ObjectFactory;
		friend class MapSaver;
};

#endif // SpringObject_H   
