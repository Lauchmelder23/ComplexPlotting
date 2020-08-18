#pragma once

#include <SDL.h>

#include "Window.hpp"

using namespace sf;

class PlotWindow :
	public IWindow
{
public:
	PlotWindow(std::string title);

	void OnEvent(const SDL_Event& e) override;
	bool OnUpdate(double frametime) override;
	void OnRender(SDL_Renderer* renderer) override;

private:
};