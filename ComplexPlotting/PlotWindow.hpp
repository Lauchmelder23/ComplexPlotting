#pragma once

#include <SDL.h>

#include "Window.hpp"

using namespace sf;

class PlotWindow :
	public IWindow
{
public:
	PlotWindow(Uint32 id);

	bool OnEvent(const SDL_Event& e) override;
	bool OnUpdate(double frametime = 0) override;
	void OnRender(SDL_Renderer* renderer = nullptr) override;

private:
	Uint32 id;
};