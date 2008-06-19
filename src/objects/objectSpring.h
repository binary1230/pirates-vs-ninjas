#ifndef SPRING_OBJECT_H
#define SPRING_OBJECT_H

#include "stdafx.h"

#include "object.h"

class ObjectFactory;

#define DEFAULT_SPRING_STRENGTH_X 0.0f
#define DEFAULT_SPRING_STRENGTH_Y 20.0f

//! A "simple" Object (e.g. scenery) - No collisions
class SpringObject : public Object {
	protected:
		uint spring_reset_time;
		bool spring_is_active;

		// When an object hits the spring, it's 
		// velocity will be set to this value
		Vector2D spring_vector;
				
	public:
		bool Init();
		void Shutdown();
		void Update();

		SpringObject();
		~SpringObject();

		void OnCollide(Object* obj, const b2ContactPoint* pkContactPoint);
		bool IsSpringActive() {return spring_is_active;};

		const Vector2D& GetSpringVector() const {return spring_vector;};

		friend class ObjectFactory;
		friend class MapSaver;
};

#endif // SpringObject_H   
