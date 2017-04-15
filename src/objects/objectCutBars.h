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

class CutBarObject : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}
		protected:
			std::string txt;				// the text to show
			CutBarState state;
			float real_pos;			// the real position of the object

			float rate;					// the rate at which it moves
			int max_size;				// the maximum size of the bar in px
			
			int time_to_show;		// amount of time to show this before fading
			int time_active;		// amount of time this has been active

			int box_alpha;					// alpha for boxes
			
		public:
			void Clear();

			virtual bool LoadObjectProperties(XMLNode & xDef);

			virtual bool Init();
			void Shutdown();

			void Start();
			void Stop();

			inline void SetText(const std::string &text) {txt = text;}

			void Update();
			void Draw();

			CutBarObject();
			virtual ~CutBarObject();
};

#endif // OBJECT_CUTBARS
