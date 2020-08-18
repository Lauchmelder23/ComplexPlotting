#include <stdio.h>
#include <SDL.h>

#undef main

void PrintSDLVersion()
{
	SDL_version* v = new SDL_version;
	SDL_VERSION(v);
	printf("Running on SDL %i.%i.%i\n", v->major, v->minor, v->patch);
	
	delete v;
	v = nullptr;
}

int main(int argc, char** argv)
{
	PrintSDLVersion();
	
	return 0;
}