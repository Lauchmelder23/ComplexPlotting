#include <stdio.h>
#include <vector>
#include <iostream>

#include "SDL.h"
#include "PlotWindowManager.hpp"

#undef main

int main(int argc, char** argv)
{
	printf("Running on SDL %i.%i.%i\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
	
	SDL_Init(SDL_INIT_VIDEO);

	// Wait for window processes to end
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;

			PlotWindowManager::HandleEvents(e);
		}

		PlotWindowManager::Update();

		PlotWindowManager::Render();
	}

	PlotWindowManager::Quit();

	return 0;
}