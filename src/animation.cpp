#include "animation.h"
#include "gameState.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"

#include <map>
using namespace std;

bool Animation::Init() {
	frames.clear();
	currentFrame = NULL;
	elapsed_time = 0;
	freeze_animation = false;
	speed_multiplier = 1;
	return true;
}

// Draw current frame at specified position, flipping if requested
void Animation::DrawAt(int x, int y, bool flip_x, bool flip_y) {
	assert(currentFrame != NULL);
	WINDOW->DrawSprite(currentFrame->sprite, x, y);
}

//! Update the animation, advancing to the next frame
//! if enough time has passed
void Animation::Update() {

	// don't need to do anything if we have less than 2 frames or are frozen
	if (frames.size() < 2 || freeze_animation)
		return;

	elapsed_time++;

	// if it's time to advance to the next frame..
	if (elapsed_time >= currentFrame->duration * speed_multiplier) {
					
		elapsed_time = 0;
		
		// NOTE: if no currentFrame->nextFrame, NEVER advance until reset
		if (currentFrame->nextFrame)
			currentFrame = currentFrame->nextFrame;
	
		if (currentFrame->freeze_at_end) {
			elapsed_time=currentFrame->duration * speed_multiplier;
			freeze_animation = true;
		}
	}
}

//! Reset this animation back to the first frame
void Animation::ResetAnimation() {
	freeze_animation = false;
	if (frames.size())
		currentFrame = frames[0];
	else
		currentFrame = NULL;
}

//! Free memory associated with this animation
void Animation::Shutdown() {
	int i, max = frames.size();

	// DO NOT FREE SPRITES

	for (i = 0; i < max; i++) {
		if (frames[i]) {
			delete frames[i];
			frames[i] = NULL;
		}
	}
}

//XXX kind of hack-ish, probably should be rewritten
//! Add another frame to this animation
//! Use for temporary loading routines, need to rethink this one.
//! Returns false on error
bool Animation::PushImage(	const char* _file, 
														const int duration, 
														bool freeze_at_end, 
														bool use_alpha) {
				
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);

	f->sprite = ASSETMANAGER->LoadSprite(_file, use_alpha);

	if (!f->sprite) {
		fprintf(stderr, "ERROR: Can't load image file: '%s'\n", _file);
		return false;
	}

	f->sprite->x_offset = 0;
	f->sprite->y_offset = 0;
	f->sprite->use_alpha = use_alpha;
	f->nextFrame = NULL;
	f->duration = duration;
	f->freeze_at_end = freeze_at_end;
	
	frames.push_back(f);

	f->nextFrame = frames[0];
	if (frames.size() > 1) {
		int last = frames.size() - 1;
		frames[last - 1]->nextFrame = f;
	} else {
		currentFrame = frames[0];
	}

	return true;
}

// XXX factory method memory leak
// XXX memory leak if Init fails, anim never gets freed
// EVERYTHING allocated like this (and objects) need to be fixed

//! Static helper method to create new animations from XML
//! XXX Does not currently support frames out of order and
//! other wackiness.  soon enough, my young apprentice - soon enough.
Animation* Animation::New(XMLNode &xAnim) {
	Animation* anim = new Animation();
	int duration;
	CString filename;
	int freeze_at_end;
	bool use_alpha = false;			// whether we use an alpha channel
	
	if (!anim || !anim->Init() )
		return NULL;

	XMLNode xFrames, xImg;
	int i, iterator, max;
	
	xFrames = xAnim.getChildNode("frames");
	max = xFrames.nChildNode("img");
		
	// fprintf(stderr, "--- max=%i\n", max);

	// loop through each <img> tag
	for (i=iterator=0; i<max; i++) {
					
		xImg = xFrames.getChildNode("img", &iterator);
		
		if (!xImg.getAttributeInt("duration", duration)) {
			return false;
		}

		filename = xImg.getAttribute("name");
		
		if (CString("true") == CString(xImg.getAttribute("alpha_enabled"))) {
			use_alpha = true;
		}
		
		// handle 'pause' attribute
		const char* freeze = xImg.getAttribute("pause");
		if (!freeze)
			freeze_at_end = 0;
		else
			sscanf(freeze, "%i", &freeze_at_end);

		// Create the frame
		if (!anim->PushImage(filename, duration, (bool)freeze_at_end, use_alpha)) {
			anim->Shutdown();
			free(anim);
			return NULL;
		}
	}

	assert(anim->frames[0]->sprite);

	// TODO: Currently this just gets the width and height from the
	// first frame.  However, we may want to get it from any
	// frame, not just the first one.  In the future, we should make
	// that an option in XML
	anim->width = anim->frames[0]->sprite->width;
	anim->height = anim->frames[0]->sprite->height;
	
	return anim;
}

Animation::Animation() {
	currentFrame = NULL;
}

Animation::~Animation() {}
