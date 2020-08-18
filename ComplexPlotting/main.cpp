#include <stdio.h>
#include <SDL.h>

#undef main

int main(int argc, char** argv)
{
	printf("Running on SDL %i.%i.%i\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
	
	return 0;
}