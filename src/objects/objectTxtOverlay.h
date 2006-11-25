#ifndef OBJ_TXTOVERLAY_H
#define OBJ_TXTOVERLAY_H

#include "stdafx.h"
#include "object.h"

class Sprite;
class ObjectFactory;
class PhysSimulation;

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
	protected:
		Sprite* avatar_sprite;
		
		vector<CString> page_texts;		// holds each "page" full of text
		uint text_index;							// which "page" we're on

		int box_color; 
		int box_margin; 
		int box_height; 

		bool is_modal;

	public:
		bool Init(PhysSimulation *p);
		void Shutdown();

		void Update();
		void Draw();

		bool SetAvatarFilename(CString file);
		void SetModalActive(bool state);

		void SetText(CString txt);

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ObjectText();
		~ObjectText();

		friend class ObjectFactory;
};

#endif // OBJ_TXTOVERLAY_H
