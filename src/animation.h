#ifndef ANIMATION_H
#define ANIMATION_H

#include "stdafx.h"

class Sprite;

#include "globals.h"
#include "gameBase.h"
#include "StdString.h"

//! Maps an animation name to an integer ID (e.g. "bad_guy_1" to BAD_GUY_1)
typedef map<const CString, uint> AnimationMapping;

//! An animation frame.  Each Animation is an array of these.
struct AnimFrame {
	Sprite* sprite;				//! Sprite data
	
	int duration;					//! Number of frames to show before advancing to next
	AnimFrame* nextFrame;	//! Pointer to next frame, or NULL at end frame
	bool freeze_at_end;		//! True if we freeze at the end of this frame
};

//! Holds sprites and displays them in a preset order and timing

//! This class holds a specific series of images that an object can use.
//! Typically, objects have a few different 'animations', for example,
//! a player object might have 2 animations - a walking and jumping animation
//! EACH of these would be seperate instances of the animation class.
class Animation : public GameBase {
	protected:
		//! Collection of frames in this animation
		vector<struct AnimFrame*> frames;	
		
		//! Points to the current frame we are drawing
		AnimFrame* currentFrame;	

		//! The number of times that our current frame has been 
		int elapsed_time;
	
		bool freeze_animation;		//! True if we do not advance the animation

		int speed_multiplier;			//! Factor to multiply the current animation 
															//! speed by. (e.g 2 = 2x as slow)
		
		int width, height;				//! Animation width/height
		
	public:
		void DrawAt(int _x, int _y, bool flip_x=0, bool flip_y=0);
		void Update();

		//! The speed multiplier can slow down the animation speed.
		//! Higher values result in a slower animation
		void SetSpeedMultiplier(int m) {speed_multiplier = max(m, 1);};

		void ResetAnimation();		//! Set the animation back to the first frame

		inline void Unfreeze() {freeze_animation = false;};
		inline void Freeze() {freeze_animation = true;};
		inline void ToggleFreeze() {freeze_animation = !freeze_animation;};

		bool Init(GameState*);		//! Initialize the animation
		void Shutdown();					//! Cleanup this animation

		//! Used in constructing a new animation, pushes this image onto it.
		bool PushImage(const char* filename, const int duration, bool freeze_at_end);
		
		inline int GetWidth() {return width;};
		inline int GetHeight() {return height;};
		
		inline Sprite* GetCurrentSprite() {return currentFrame->sprite;}

		Animation();
		~Animation();

		//! Static factory method
		static Animation* New(GameState* gameState, XMLNode &xAnim);
};

#endif // ANIMATION_H
