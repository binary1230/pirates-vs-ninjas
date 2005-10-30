/* A force vector
 * all time increments are 1/60th of a second
 */

#ifndef FORCE_H
#define FORCE_H

class Force;
class GameState;

#include "globals.h"
#include "gameBase.h"

class Force : public GameBase {
	public:
		void Init(GameState* _game_state);
		virtual float GetAcceleration() = 0;
		void Shutdown();

		Force();
		virtual ~Force();
};

#endif // FORCE_H
