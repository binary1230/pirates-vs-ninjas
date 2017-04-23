#ifndef OBJ_TXTOVERLAY_H
#define OBJ_TXTOVERLAY_H

#include "object.h"

class Sprite;
class ObjectFactory;

//! A text box that can show an avatar
//! It can be "modal" which means that nothing can happen in the game
//! until the user presses a button
//! It can be multi-page and multi-lines per screen.
//
// To tell the text to go to the next line, use a pipe '|'
// character (located above the backslash), and to seperated "pages" of
// text, use the tilda '~'
//
// For example:
// Setting the text to "Wait a sec!|Something is wrong~Oh no!!|We're doomed"
// will make a textbox that displays:
//
// --------------------
// Wait a sec!
// Something is wrong
// --------------------
//
// and when the user presses a button, it will go to the next page 
// which will show:
//
// --------------------
// Oh no!!
// We're doomed
// --------------------
//
// When the user presses a key again, it will close the textbox
class ObjectText : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:
		Sprite *avatar_sprite, *wait_sprite;

		int time_until_next_blink;
		bool blink;
		
		vector<std::string> page_texts;		// holds each "page" full of text
		uint text_index;							// which "page" we're on

		ALLEGRO_COLOR box_color; 
		int box_margin; 
		int box_height; 

		bool is_modal;

	public:
		IMPLEMENT_CLONE(ObjectText)

		bool Init();
		void Shutdown();

		void Clear();

		void Update();
		void Draw();

		bool SetAvatarFilename(std::string file);
		void SetModalActive(bool state);

		virtual bool LoadXMLInstanceProperties(XMLNode & xObj);
		virtual bool LoadObjectProperties(XMLNode & xDef);

		void SetText(std::string txt);

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ObjectText();
		~ObjectText();

		friend class ObjectFactory;
};

#endif // OBJ_TXTOVERLAY_H
