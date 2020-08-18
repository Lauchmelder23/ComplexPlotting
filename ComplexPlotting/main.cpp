#include <stdio.h>
#include <vector>

#include <SDL.h>
#include "PlotWindow.hpp"

#undef main

int main(int argc, char** argv)
{
	printf("Running on SDL %i.%i.%i\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
	
	SDL_Init(SDL_INIT_VIDEO);

	// Create and open some windows
	PlotWindow window("Plot");

	// Wait for window processes to end
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;

			window.OnEvent(e);
		}

		window.OnUpdate(0);

		window.OnRender(nullptr);
	}

	return 0;
}