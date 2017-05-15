#include "stdafx.h"
#include "animation.h"
#include "game.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"
#include "gameSound.h"
#include "effectsManager.h"
#include "objects/object.h"

AnimFrame::AnimFrame() { Clear(); }
AnimFrame::~AnimFrame() { Clear(); }

void AnimFrame::Clear() 
{
	frame_type = ANIMFRAME_INVALID;
	sprite = NULL;
	duration = -1;
	freeze_at_end = false;
	nextFrame = NULL;
}

// Draw current frame at specified position, flipping if requested
void Animation::DrawAt(int x, int y, bool flip_x, bool flip_y) 
{
	assert(_currentFrame != NULL);
	WINDOW->DrawSprite(_currentFrame->sprite, x, y);
}

void Animation::SetSpeedMultiplier(int multiplier) {
	_speed_multiplier = max(multiplier, 1);
}

void Animation::SetFrozen(bool is_frozen) {
	_freeze_animation = is_frozen; 
	if (!_freeze_animation) {
		_animation_just_started = false;
	}
}

//! Update the animation, advancing to the next frame if enough time has passed
void Animation::Update() 
{
	// don't need to do anything if we don't have more than 1 frame, or are frozen
	if (frames.size() <= 1 || _freeze_animation)
		return;

	--_time_til_next_frame;

	if (_time_til_next_frame <= 0 || _animation_just_started) {
		if (!_animation_just_started) {
			AdvanceOneFrame();
		}

		_animation_just_started = false;
		RunCurrentFrameActions();

		_time_til_next_frame = _currentFrame->duration * _speed_multiplier;

		if (_currentFrame->freeze_at_end) {
			_time_til_next_frame = 0; // when we unfreeze, immediately go to next frame
			_freeze_animation = true;
		}
	}
}

void Animation::SwitchToNextFrame()
{

}

// return true if caller should immediately process the next frame
bool Animation::RunCurrentFrameAction() {
	bool should_run_next_frame_action = false;

	switch (_currentFrame->frame_type) {
	case ANIMFRAME_SPRITE:
		// noop
		break;

	case ANIMFRAME_DESTROY:
		if (_attachedObject)
			_attachedObject->SetIsDead(true);
		break;

	case ANIMFRAME_SOUND:
		if (_currentFrame->extraData.length() == 0)
			TRACE("ERROR: No sound name specified in animation sound frame\n");
		else
			SOUND->PlaySound(_currentFrame->extraData);

		should_run_next_frame_action = true;
		break;

	case ANIMFRAME_EFFECT:
		if (_currentFrame->extraData.length() == 0)
			TRACE("ERROR: No effect name specified in animation sound frame\n");
		else
			EFFECTS->TriggerEffect(_attachedObject, _currentFrame->extraData);

		should_run_next_frame_action = true;
		break;

	case ANIMFRAME_EXPLOSION:
		PHYSICS->CreateExplosionAt(_attachedObject->GetCenter());
		should_run_next_frame_action = true;
		break;

	case ANIMFRAME_JUMP:
		// just go to the next frame
		should_run_next_frame_action = true;
		break;

	default: case ANIMFRAME_INVALID:
		assert(0 && "ERROR: invalid frame type!");
		break;
	}

	return should_run_next_frame_action;
}

void Animation::AdvanceOneFrame() {
	// actually advance to next frame, if one exists
	// if next frame doesn't exist, don't advance it until we're reset
	if (_currentFrame->nextFrame)
		_currentFrame = _currentFrame->nextFrame;
}

// Switch this animation to its next frame, do the action of that frame
void Animation::RunCurrentFrameActions()
{
	bool continue_advancing_frames;
	int safety__max_loops_remaining = 100;

	do {
		continue_advancing_frames = RunCurrentFrameAction();

		if (continue_advancing_frames)
			AdvanceOneFrame();

		if (_currentFrame == frames[0] && _attachedObject)
			_attachedObject->OnAnimationLooped();

		// sanity check, make sure we aren't in an infinite loop
		safety__max_loops_remaining--;
		assert(safety__max_loops_remaining != 0 && "Animation data error: We're in an infinite loop, check animation data for problems");
		if (safety__max_loops_remaining == 0)
			break;

	} while (continue_advancing_frames);

	// make sure we ended up on a valid frame type
	assert(_currentFrame->frame_type == ANIMFRAME_SPRITE || _currentFrame->frame_type == ANIMFRAME_DESTROY);
}

//! Reset this animation back to the first frame
void Animation::ResetAnimation() 
{
	_freeze_animation = false;
	_animation_just_started = true;

	if (frames.size())
		_currentFrame = frames[0];
	else
		_currentFrame = NULL;

	
}

//! Free memory associated with this animation
void Animation::Shutdown() 
{
	int i, max = frames.size();

	// DO NOT FREE SPRITES

	for (i = 0; i < max; i++) {
		if (frames[i]) {
			delete frames[i];
			frames[i] = NULL;
		}
	}
}


bool Animation::CreateSpriteFrame(	const char* file, const int duration, bool freeze_at_end) {
	// see if this filename has a sequence indicated on it

	string filename = file;

	int idx = filename.find("XXXX");

	if (idx == string::npos) {
		// no sequence present, just load this file directly
		if (CreateSingleSpriteFrame(file, duration, freeze_at_end)) {
			return true;	
		} else {
			TRACE("ERROR: Can't load image file: '%s'\n", file);
			return false;
		}
	}
	
	// animation sequence of sprites present on disk (i.e. frame0001.png, frame0002.png, etc), try to load there
	int frame_num = 1; // start on frame 1
	bool loaded_this_ok;
	bool loaded_at_least_one_frame = false;
	string final_filename;

	do {
		std::ostringstream ss;
		ss << std::setw(4) << std::setfill('0') << frame_num;
		final_filename = filename.substr(0, idx) + ss.str() + filename.substr(idx + 4);

		loaded_this_ok = CreateSingleSpriteFrame(final_filename.c_str(), duration, freeze_at_end);
		loaded_at_least_one_frame = loaded_at_least_one_frame || loaded_this_ok;

		frame_num++;
	} while (loaded_this_ok);

	return loaded_at_least_one_frame;
}

bool Animation::CreateSingleSpriteFrame(const char* file, const int duration, bool freeze_at_end) {
	assert(file != NULL);

	Sprite* sprite = ASSETMANAGER->LoadSprite(file);

	if (!sprite) {
		return false;
	}

	AnimFrame *f = new AnimFrame();

	f->sprite = sprite;
	f->frame_type = ANIMFRAME_SPRITE;
	f->duration = duration;
	f->freeze_at_end = freeze_at_end;

	return PushFrame(f);
}

bool Animation::CreateEffectFrame(	const std::string &effectData, 
									const bool freeze_at_end ) 
{	
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);
	assert(effectData.length() > 0);

	f->frame_type = ANIMFRAME_EFFECT;
	f->duration = 0;
	f->extraData = effectData;

	return PushFrame(f);
}


bool Animation::CreateExplosionFrame()
{
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);

	f->frame_type = ANIMFRAME_EXPLOSION;
	f->duration = 0;
	f->extraData = "";

	return PushFrame(f);
}


bool Animation::CreateSoundFrame(	const std::string &soundData, 
									const bool freeze_at_end	) 
{	
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);
	assert(soundData.length() > 0);

	f->frame_type = ANIMFRAME_SOUND;
	f->duration = 0;
	f->extraData = soundData;

	return PushFrame(f);
}

bool Animation::CreateDestroyFrame()
{
	AnimFrame* f = new AnimFrame();
	assert(f != NULL);

	f->frame_type = ANIMFRAME_DESTROY;
	f->duration = 0;
	f->extraData = "";

	return PushFrame(f);
}

//! Add a JUMP frame to this animation
//! Think of this like a goto statement -> the next frame will loop back on a previous frame
//! iFrameToJumpTo is an INDEX starting with ZERO.  The XML is a frame NUMBER that starts at ONE
bool Animation::CreateJumpFrame( int iFrameToJumpTo ) 
{				
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);

	f->sprite = NULL;
	f->frame_type = ANIMFRAME_JUMP;
	f->duration = 0;

	if (!PushFrame(f))
		return false;

	// we are now the last frame of the sequence
	int me = frames.size() - 1;
	
	if (iFrameToJumpTo < 0 || iFrameToJumpTo >= (int)frames.size())
	{
		TRACE("ERROR: A jump frame wants to jump to jump to an illegal frame number: %i", iFrameToJumpTo);
		return false;
	}
	else if (iFrameToJumpTo == me || frames[iFrameToJumpTo] == f)
	{
		TRACE("ERROR: A jump frame tried to jump back to itself -> infinite loop");
		return false;
	}
	else if (frames[iFrameToJumpTo]->frame_type == ANIMFRAME_JUMP)
	{
		TRACE("ERROR: A jump frame wants to jump to ANOTHER jump frame (disallowed for now), frame number: %i", iFrameToJumpTo);
		return false;
	}

	// now that we're all set, let's switch the pointers up
	f->nextFrame = frames[iFrameToJumpTo];
	return true;
}

bool Animation::PushFrame(AnimFrame* f) 
{
	frames.push_back(f);

	f->nextFrame = frames[0];

	// frames->nextFrame links to the frame to play after this one.
	// ensure that the last frame always links to the first one
	if (frames.size() <= 1) {
		_currentFrame = frames[0];
 	} else {
		int lastFrame = frames.size() - 1;
		frames[lastFrame - 1]->nextFrame = f;
	}

	return true;
}

bool Animation::Init(XMLNode &xAnim, Object* attachedObject)
{
	std::string sprite_filename, frame_type, extraData;
	XMLNode xFrames, xFrame;
	int i, iterator, numFrames, numSpriteFrames = 0;
	int first_sprite_frame = -1; // which frame is the first sprite frame
	
	_animation_just_started = true;
	frames.clear();
	_currentFrame = NULL;
	_time_til_next_frame = 0;
	_freeze_animation = false;
	_speed_multiplier = 1;
	_attachedObject = attachedObject;
	
	xFrames = xAnim.getChildNode("frames");
	numFrames = xFrames.nChildNode("frame");

	assert(numFrames != 0 && "ERROR: No <frame> tags found!\n");

	bool loaded_ok = true;

	// loop through each <frame> tag
	for (i=iterator=0; i<numFrames; i++) 
	{
		xFrame = xFrames.getChildNode("frame", &iterator);
	
		int freeze_at_end;
		const char* freeze = xFrame.getAttribute("pause");
		if (!freeze)
			freeze_at_end = 0;
		else
			sscanf(freeze, "%i", &freeze_at_end);

		// if this frame has a "type" attribute, use it, if not assume it's a
		// of type "sprite" (as opossed to "sound" and "effect" types)
		if (xFrame.getAttribute("type") == NULL)
			frame_type = "sprite";
		else
			frame_type = xFrame.getAttribute("type");

		// figure out what type of frame this is and do The Right Thing
		if (frame_type == "sprite") 
		{
			// normally what happens - you get an image filename
			sprite_filename = xFrame.getAttribute("name");

			int duration;
			loaded_ok = xFrame.getAttributeInt("duration", duration);
			loaded_ok = loaded_ok && CreateSpriteFrame(sprite_filename.c_str(), duration, freeze_at_end != 0);

			if (first_sprite_frame == -1)
				first_sprite_frame = i;

			++numSpriteFrames;

		} 
		else if (frame_type == "effect")
		{
			// effect frames don't display anything but instead trigger effects 
			// such as smoke/dust/etc
			extraData = xFrame.getAttribute("data");
			assert(extraData.length() != 0);

			loaded_ok = CreateEffectFrame(extraData, freeze_at_end != 0);
		} 
		else if (frame_type == "sound") 
		{
			// sound frames don't display anything but instead trigger sounds 
			extraData = xFrame.getAttribute("data");
			assert(extraData.length() != 0);

			loaded_ok = CreateSoundFrame(extraData, freeze_at_end != 0);
		}
		else if (frame_type == "explosion") {
			loaded_ok = CreateExplosionFrame();
		}
		else if (frame_type == "destroy") 
		{
			loaded_ok = CreateDestroyFrame();
		} 
		else if (frame_type == "jumpToFrame")
		{
			int iFrameIndexToJumpTo;

			loaded_ok = xFrame.getAttributeInt("num", iFrameIndexToJumpTo);
			iFrameIndexToJumpTo--; // subtract one since this is an INDEX not a FRAME NUMBER
			loaded_ok = loaded_ok && CreateJumpFrame(iFrameIndexToJumpTo);
		}
		else 
		{
			TRACE("ERROR: Invalid frame type specified: '%s'\n", frame_type.c_str());
		}

		if (!loaded_ok)
			break;
	}

	if (!loaded_ok) {
		Shutdown();
		return false;
	}

	// hack.
	assert(numSpriteFrames >= 1 && 	"ERROR: Animations must have at least 1\n"
									"       sprite frame or else it will hang\n"
									"       when played back!" );

	// TODO: Currently this just gets the width and height from the
	// first frame.  However, we may want to get it from any
	// frame, not just the first one.  In the future, we should make
	// that an option in XML
	assert(first_sprite_frame != -1);

	_width = frames[first_sprite_frame]->sprite->width;
	_height = frames[first_sprite_frame]->sprite->height;

	if (_currentFrame->freeze_at_end)
		_freeze_animation = true;
	
	return true;
}

Animation::Animation()
{
	_debug_flag = false;
	_currentFrame = NULL;
	_attachedObject = NULL;
	_animation_just_started = true;
	_time_til_next_frame = 0;
	_freeze_animation = false;
	_speed_multiplier = 1;
	_width = _height = -1;
}

Animation::~Animation() {}
