#include "stdafx.h"
#include "objectCutBars.h"
#include "window.h"
#include "rect.h"
#include "globalDefines.h"

bool CutBarObject::Init() {
	real_pos = 0.0f;
	time_active = 0;

	// Load default values from global XML
	if (	!GLOBALS->Value("cutbar_rate", rate) ||
				!GLOBALS->Value("cutbar_maxsize", max_size) ||
				!GLOBALS->Value("cutbar_alpha", alpha) ||
				!GLOBALS->Value("cutbar_time_to_show", time_to_show)) {
		return false;
	}

	return BaseInit();
}

void CutBarObject::Shutdown() {
	BaseShutdown();
}

void CutBarObject::Update() {

	switch (state) {

		case STATE_INACTIVE:
			// do nothing
			break;

		case STATE_ROLL_IN:
			real_pos += rate;
			if (real_pos >= max_size) {
				real_pos = max_size;
				state = STATE_ACTIVE;
				time_active = 0;
			}
			break;

		case STATE_ACTIVE:
			++time_active;
			if (time_active >= time_to_show)
				state = STATE_ROLL_OUT;
			break;

		case STATE_ROLL_OUT:
			real_pos -= rate;
			if (real_pos <= 0) {
				real_pos = 0;
				state = STATE_INACTIVE;
				Stop();
			}
			break;
	}
}

void CutBarObject::Draw() {
	if (state == STATE_INACTIVE)
		return;

	int screen_height = WINDOW->Height();
	int screen_width = WINDOW->Width();

	// bottom bar
	WINDOW->DrawRect(	0, 0,
										screen_width, (int)real_pos,
										makecol(0,0,0), true, alpha );

	// top bar
	WINDOW->DrawRect(	0, screen_height,
										screen_width, screen_height - (int)real_pos,
									  makecol(0,0,0), true, alpha );

	// text
	if (state == STATE_ACTIVE)
		WINDOW->DrawText(30, screen_height - max_size + 2, txt);
}

void CutBarObject::Stop() {
	state = STATE_INACTIVE;
	is_dead = true;
}

void CutBarObject::Start() {
	state = STATE_ROLL_IN;
	real_pos = 0;
}

CutBarObject::CutBarObject() {
	txt = "PLACEHOLDER";
	state = STATE_INACTIVE;
}

CutBarObject::~CutBarObject() {}
