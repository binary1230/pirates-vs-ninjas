#include "stdafx.h"
#include "objectCutBars.h"
#include "window.h"
#include "rect.h"
#include "globalDefines.h"
#include "gameWorld.h"

bool ObjectCutBars::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	is_overlay = true;

	return true;
}

bool ObjectCutBars::Init() {
	// Load default values from global XML
	if (!GLOBALS->Value("cutbar_rate", _rate) ||
		!GLOBALS->Value("cutbar_maxsize", _max_size) ||
		!GLOBALS->Value("cutbar_alpha", _box_alpha) ||
		!GLOBALS->Value("cutbar_time_to_show", _time_to_show)) {
		return false;
	}

	return BaseInit();
}

void ObjectCutBars::Shutdown() {
	BaseShutdown();
}

void ObjectCutBars::Update() {

	switch (_state) {

		case STATE_INACTIVE:
			// do nothing
			break;

		case STATE_ROLL_IN:
			_real_pos += _rate;
			if (_real_pos >= _max_size) {
				_real_pos = _max_size;
				_state = STATE_ACTIVE;
				_time_active = 0;
			}
			break;

		case STATE_ACTIVE:
			++_time_active;
			if (_time_active >= _time_to_show)
				_state = STATE_ROLL_OUT;
			break;

		case STATE_ROLL_OUT:
			_real_pos -= _rate;
			if (_real_pos <= 0) {
				_real_pos = 0;
				_state = STATE_INACTIVE;
				Stop();
			}
			break;
	}
}

void ObjectCutBars::Draw() {
	if (_state == STATE_INACTIVE)
		return;

	// don't draw this textbox if there's a modal object present that's not us
	if (WORLD->GetModalObject() && WORLD->GetModalObject() != this)
		return;

	int screen_height = WINDOW->Height();
	int screen_width = WINDOW->Width();

	// bottom bar
	WINDOW->DrawRect(	0, 0,
						screen_width, (int)_real_pos,
						al_map_rgb(0,0,0), true, _box_alpha );

	// top bar
	WINDOW->DrawRect(	0, screen_height,
						screen_width, screen_height - (int)_real_pos,
						al_map_rgb(0,0,0), true, _box_alpha );

	// text
	if (_state == STATE_ACTIVE)
		WINDOW->DrawText(30, screen_height - _max_size + 2, _txt);
}

void ObjectCutBars::Stop() {
	_state = STATE_INACTIVE;
	is_dead = true;
}

void ObjectCutBars::Start() {
	_state = STATE_ROLL_IN;
	_real_pos = 0;
}

void ObjectCutBars::Clear() {
	Object::Clear();

	_txt = "PLACEHOLDER";
	_real_pos = 0.0f;
	_time_active = 0;

	_state = STATE_INACTIVE;

	_rate = 1.0f;
	_max_size = 0;
	_time_to_show = 0;
	_time_active = 0;

	_box_alpha = 255;
}

ObjectCutBars::ObjectCutBars() {
	Clear();
}

ObjectCutBars::~ObjectCutBars() {}

BOOST_CLASS_EXPORT_GUID(ObjectCutBars, "ObjectCutBars")