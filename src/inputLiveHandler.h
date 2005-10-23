/* Live Input 
 *
 * Repsonsible for handling 'live' input (keyboard, mouse, etc)
 */

#ifndef INPUTLIVEHANDLER_H
#define INPUTLIVEHANDLER_H

#include <allegro.h>

#include "globals.h"
#include "input.h"

class InputLive : public BaseInput {
	public:
		int Init();
		void Shutdown();
				
		bool Key(unsigned char index);
		int MouseX();
		int MouseY();

		InputLive();
		~InputLive();
};

#endif // INPUTLIVEHANDLER_H
