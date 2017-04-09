#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

class Object;

//! A container class representing "layers" (e.g. background, foreground, etc)

//! A layer holds pointers to the objects on it. Its main purpose is to 
//! allow Object's to be drawn in the correct order on-screen and allow
//! multiple scrolling speeds
class ObjectLayer {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */)
	{
		ar	& BOOST_SERIALIZATION_NVP(objects)
			& BOOST_SERIALIZATION_NVP(scroll_speed);
			// & BOOST_SERIALIZATION_NVP(name);
	}

	protected:
		list<Object*> objects;
		bool visible;
		float scroll_speed;
		CString name;
	
	public:
		bool Init();
		void Shutdown();

		void SetName(const char* _name) {name = _name;}
		const char* GetName() {return name.c_str();}

		//! Get/set layer scroll speed
		void SetScrollSpeed(float _scroll_speed) { scroll_speed = _scroll_speed ;};
		float GetScrollSpeed() { return scroll_speed; };

		//! Get/Set layer visibility
		bool IsVisible() { return visible; };
		void SetVisible(bool _visible) { visible = _visible; };

		//! Draw this layer with camera scroll take into effect
		void Draw();
		
		//! Put an object onto this layer
		void AddObject(Object*);

		//! Remove an object from this layer
		void RemoveObject(Object*);
		
		ObjectLayer();
		virtual ~ObjectLayer();

		friend class MapSaver;
};

#endif // OBJECTLAYER_H
