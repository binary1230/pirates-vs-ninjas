#include "objectPlayer.h"

void PlayerObject::SetInputHandler(BaseInput* handler) {
	input = handler;
}

void PlayerObject::Update() {	
	x = vectors[_X].CalcNextStep();
	y = -vectors[_Y].CalcNextStep();

	if (y > dst_bitmap->h - bitmap->h) {
			y = dst_bitmap->h - bitmap->h;
			vectors[_Y].position = -y;
	}
		
	vectors[_X].v_decay = 1.00f;	// no decay in the air

	// if we're OK to jump now.. do it.
	if (y == dst_bitmap->h - bitmap->h) {
		vectors[_X].v_decay = 0.99f;	// decay on the ground
		if (input->Key(GAMEKEY_JUMP)) {
			vectors[_Y].velocity = Rand(2,12);
	  }		
	}
	
	if (x < 0) {
			vectors[_X].position = x = 0;
			vectors[_X].velocity = -vectors[_X].velocity;
	}
	
	if (x > dst_bitmap->w - bitmap->w) {
			vectors[_X].position = x = dst_bitmap->w - bitmap->w;
			vectors[_X].velocity = -vectors[_X].velocity;
	}
}

void PlayerObject::Draw() {
	
	bool flip = false;
	
	if (vectors[_X].acceleration == 0) {
		if (vectors[_X].velocity > 0) {
			flip = true;
		}
	} else if (vectors[_X].acceleration > 0) {
		flip = true;
	} 

	if (flip)
		draw_sprite_h_flip(dst_bitmap, bitmap, x, y);
	else 
		draw_sprite(dst_bitmap, bitmap, x, y);
}

void PlayerObject::SetXY(int _x, int _y) {
	x = _x; 
	y = _y;
	vectors[_Y].position = (float)y;
	vectors[_X].position = (float)x;
	fprintf(stderr, "setting them!\n");
}

bool PlayerObject::Init(GameState* _game_state) {
	Force* force;

	SetGameState(_game_state);

	assert(GetGameState() != NULL);

	vectors[_X].Clear();
	vectors[_Y].Clear();

	vectors[_X].v_decay = 0.99f;

	force = new ForceInput();
	force->Init(GetGameState());
	vectors[_X].Attach(force);

	force = new ForceGravity();
	force->Init(GetGameState());
	vectors[_Y].Attach(force);

	return true;
}

void PlayerObject::Shutdown() {
	// XXX free vector forces
	// a.k.a vectors[x].Shutdown()
}

PlayerObject::PlayerObject() : input(NULL) {
}

PlayerObject::~PlayerObject() {}
