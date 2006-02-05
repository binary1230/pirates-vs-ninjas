#include <SDL/SDL.h>   
#include <SDL/SDL_image.h>
#include <stdlib.h>

int main(int argc, char *argv[])   {

   Uint32 flags = SDL_SWSURFACE;//|SDL_FULLSCREEN;
   if(SDL_Init(SDL_INIT_VIDEO) < 0)   {
      return -1;
   }

	SDL_Surface * screen;
	SDL_Surface * image;

	screen = SDL_SetVideoMode(640, 480, 32, flags);
	//image  = SDL_LoadBMP("image.bmp");  //load bmp using SDL
	image  = IMG_Load("image.bmp"); 		//load jpg  using SDL_image

	SDL_SetColorKey(image, SDL_SRCCOLORKEY|SDL_RLEACCEL, 
									SDL_MapRGB(image->format, 255, 0, 255));
	
	/* Puting image on the screen up*/
	SDL_BlitSurface(image, NULL, screen, NULL);
	SDL_Flip(screen);
	SDL_Delay(4000);
} 
