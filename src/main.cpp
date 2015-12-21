#include "SDL/SDL.h"

int main(int argc, char* args[])
{
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Surface *screen;

    int width = 320;
    int height = 160;

    /* Creating the surface. */
    screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_Flip(screen);
    
    SDL_Delay(5000);

    // Quit SDL
    SDL_Quit();

    return 0;
}
