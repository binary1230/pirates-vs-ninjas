#include "force.h"

bool Force::Init(GameState* _game_state) {
	return (game_state = _game_state) != NULL;
}

void Force::Shutdown() {
}

Force::Force() {}
Force::~Force() {}
