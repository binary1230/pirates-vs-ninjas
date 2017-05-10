#ifndef ANIMATION_H
#define ANIMATION_H

class Sprite;
class Object;

#include "globals.h"

//! Maps an animation name to an integer ID (e.g. "bad_guy_1" to BAD_GUY_1)
typedef map<const std::string, uint> AnimationMapping;

//! What kind of frame this is
//! Sprite - this frame is an image we should display (NORMALLY what happens)
//! 
enum AnimFrameType {
	ANIMFRAME_SPRITE,   // this frame displays a sprite
	ANIMFRAME_EFFECT,	// this frame triggers an effect (dust, smoke, etc)
	ANIMFRAME_SOUND,	// this frame triggers a sound
	ANIMFRAME_EXPLOSION, // this frame triggers a physics effect
	ANIMFRAME_DESTROY,  // this frame destroyes the parent object
	ANIMFRAME_JUMP,  // this frame destroyes the parent object

	ANIMFRAME_INVALID = -1
};

//! An animation frame.  Each Animation is an array of these.
struct AnimFrame {
	int duration;					//! Number of frames to show before advancing to next
	bool freeze_at_end;		//! True if we freeze at the end of this frame

	AnimFrame* nextFrame;	//! Pointer to next frame, or NULL at end frame
	
	//! What "kind" of frame this is - usually is ANIMFRAME_SPRITE, but can
	//! also be a frame which triggers an effect or a sound
	AnimFrameType frame_type;

	// ONLY ONE of the following is used depending on frame_type.
	Sprite* sprite;			//! Sprite data if ANIMFRAME_SPRITE
	std::string extraData;		//! Extra frame data for use with ANIMFRAME_EFFECT
							//! or ANIMFRAME_SOUND

	// really just helpers.
	AnimFrame();
	~AnimFrame();
	void Clear();
};

//! Holds sprites and displays them in a preset order and timing

//! This class holds a specific series of images that an object can use.
//! Typically, objects have a few different 'animations', for example,
//! a player object might have 2 animations - a walking and jumping animation
//! EACH of these would be seperate instances of the animation class.
class Animation {
	protected:
		//! Collection of frames in this animation
		vector<struct AnimFrame*> frames;	
		
		//! Points to the current frame we are drawing
		AnimFrame* currentFrame;	

		//! Points to the object this animation is associated with
		Object* attachedObject;

		//! The number of times that our current frame has been 
		int elapsed_time;
	
		bool freeze_animation;		//! True if we do not advance the animation

		int speed_multiplier;			//! Factor to multiply the current animation 
															//! speed by. (e.g 2 = 2x as slow)
		
		int width, height;				//! Animation width/height

		// Push a new frame onto the end of this animation
		bool PushFrame(AnimFrame* f);
		
	public:
		void DrawAt(int _x, int _y, bool flip_x=0, bool flip_y=0);
		void Update();

		//! The speed multiplier can slow down the animation speed.
		//! Higher values result in a slower animation
		void SetSpeedMultiplier(int m) {speed_multiplier = max(m, 1);};

		void SwitchToNextFrame();

		void ResetAnimation();		//! Set the animation back to the first frame

		inline void Unfreeze() {freeze_animation = false;};
		inline void Freeze() {freeze_animation = true;};
		inline void ToggleFreeze() {freeze_animation = !freeze_animation;};

		bool Init();							//! Initialize the animation
		void Shutdown();					//! Cleanup this animation

		//! Used in constructing a new animation
		//! Pushes a sprite frame onto it.
		bool CreateSpriteFrame(	const char* filename, const int duration, 
								const bool freeze_at_end);

		bool CreateSingleSpriteFrame(const char * file, const int duration, bool freeze_at_end);

		//! Used in constructing a new animation 
		//! Pushes a sound frame onto it
		bool CreateEffectFrame(	const std::string &effectData, 
								const bool freeze_at_end	);

		bool CreateExplosionFrame();

		//! Used in constructing a new animation
		//! Pushes an effect frame onto it
		bool CreateSoundFrame(	const std::string &effectData, 
								const bool freeze_at_end	);

		//! Used in constructing a new animation
		//! When this frame is called we will destroy the parent object
		bool CreateDestroyFrame();

		//! Used in constructing a new animation
		//! When this frame is called we will jump to a different frame #
		bool CreateJumpFrame( int iFrameToJumpTo );
		
		inline int GetWidth() {return width;};
		inline int GetHeight() {return height;};
		
		inline Sprite* GetCurrentSprite() {return currentFrame->sprite;}

		Animation();
		~Animation();

		//! Static factory method
		static Animation* Load(XMLNode &xAnim, Object* attachedObject);
};

#endif // ANIMATION_H
