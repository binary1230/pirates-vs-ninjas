#ifndef ANIMATION_EDITOR_H
#define ANIMATION_EDITOR_H

#include "gameMode.h"
#include "sprite.h"

class GameState;

class AnimationEditorMode : public GameMode
{
		protected:		
			std::string m_sFlashText;
			Sprite* m_pkCursorSprite;

			void SetFlashText(char * format, ... );

			void LoadSprite(const char* objectXmlFile);

		public:
			int Init(XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			AnimationEditorMode();
			~AnimationEditorMode();
};

#endif // ANIMATION_EDITOR_H