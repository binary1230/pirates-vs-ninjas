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
	void serialize(Archive & ar, const unsigned int version)
	{
		if (version < 1) {
			list<Object*> _obsolete;
			ar & boost::serialization::make_nvp("objects", _obsolete);
		}

		ar & BOOST_SERIALIZATION_NVP(scroll_speed);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(visible);
	}

	protected:
		bool visible;
		float scroll_speed;
		std::string name;
	
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
		
		ObjectLayer();
		virtual ~ObjectLayer();

		friend class MapSaver;
};

#if !defined(SWIG)
BOOST_CLASS_VERSION(ObjectLayer, 1)
#endif 

#endif // OBJECTLAYER_H
