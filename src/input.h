/* Input Base Class
 *
 * The only reason for having a base class is
 * to make derived classes - one which can do 
 * real input live from keyboard, mouse, or joystick
 * and the other class to be able to record the input
 * and play it back for the sake of a demo game.
 *
 * The underlying game should not notice the difference
 * between live and recorded stuff.
 */

#ifndef INPUT_H
#define INPUT_H

#include <allegro.h>

class BaseInput {

	public:
		virtual int Init() = 0;
		virtual void Shutdown() = 0;
				
		virtual bool Key(unsigned char index) = 0;
		virtual int MouseX() = 0;
		virtual int MouseY() = 0;
	
		// for another day...
		/*virtual void load_recording(char* filename);
		virtual void begin_recording(int random_seed);
		virtual void end_recording();
		virtual void save_recording(char* filename);
		virtual void get_random_seed();*/

		BaseInput();
		virtual ~BaseInput() = 0;
};

#endif // INPUT_H
