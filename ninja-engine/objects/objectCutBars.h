#ifndef OBJECT_CUTBARS
#define OBJECT_CUTBARS

// "cut bar" - the bars that make the screen go into 16x9 mode
// (like how halo does it)

#include "object.h"

enum CutBarState {
	STATE_INACTIVE,
	STATE_ROLL_IN,
	STATE_ACTIVE,
	STATE_ROLL_OUT
};

class ObjectCutBars : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}
		protected:
			std::string _txt;				// the text to show
			CutBarState _state;
			float _real_pos;				// the real position of the object

			float _rate;					// the _rate at which it moves
			int _max_size;					// the maximum size of the bar in px
			
			int _time_to_show;				// amount of time to show this before fading
			int _time_active;				// amount of time this has been active

			int _box_alpha;					// alpha for boxes
			
		public:
			IMPLEMENT_CLONE(ObjectCutBars)

			void Clear();

			virtual bool LoadXMLInstanceProperties(XMLNode & xObj);
			virtual bool LoadObjectProperties(XMLNode & xDef);

			virtual bool Init();
			void Shutdown();

			void Start();
			void Stop();

			inline void SetText(const std::string &text) {_txt = text;}

			void Update();
			void Draw();

			ObjectCutBars();
			virtual ~ObjectCutBars();
};

#endif // OBJECT_CUTBARS
