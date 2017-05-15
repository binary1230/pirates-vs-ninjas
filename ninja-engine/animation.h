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
	ANIMFRAME_SPRITE,		// this frame displays a sprite
	ANIMFRAME_EFFECT,		// this frame triggers an effect (dust, smoke, etc)
	ANIMFRAME_SOUND,		// this frame triggers a sound
	ANIMFRAME_EXPLOSION,	// this frame triggers a physics effect
	ANIMFRAME_DESTROY,		// this frame destroys the parent object
	ANIMFRAME_JUMP,			// jump to another frame in the animation

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
		AnimFrame* _currentFrame;	

		//! Points to the object this animation is associated with
		Object* _attachedObject;

		bool _animation_just_started;

		int _time_til_next_frame;
	
		bool _freeze_animation;		//! True if we do not advance the animation

		int _speed_multiplier;			//! Factor to multiply the current animation 
										//! speed by. (e.g 2 = 2x as slow)
		
		int _width, _height;				//! Animation width/height

		// Push a new frame onto the end of this animation
		bool PushFrame(AnimFrame* f);

		bool RunCurrentFrameAction();

		void AdvanceOneFrame();
		
	public:
		void DrawAt(int _x, int _y, bool flip_x=0, bool flip_y=0);
		void Update();

		void SwitchToNextFrame();

		//! The speed multiplier can slow down the animation speed.
		//! Higher values result in a slower animation
		void SetSpeedMultiplier(int multiplier);

		void RunCurrentFrameActions();

		void ResetAnimation();		//! Set the animation back to the first frame

		void SetFrozen(bool is_frozen);
		inline void ToggleFreeze() {_freeze_animation = !_freeze_animation;}

		bool Init(XMLNode &xAnim, Object* attachedObject);
		void Shutdown();

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
		
		inline int GetWidth() {return _width;};
		inline int GetHeight() {return _height;};
		
		inline Sprite* GetCurrentSprite() {return _currentFrame->sprite;}

		Animation();
		~Animation();

		bool _debug_flag;
};

#endif // ANIMATION_H
