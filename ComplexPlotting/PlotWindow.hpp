#pragma once

#include <complex>
#include <functional>

#include <SDL.h>

#include "Window.hpp"

using namespace sf;

typedef std::function<std::complex<float>(std::complex<float>)> CmplxFunc;

class PlotWindow :
	public IWindow
{
public:
	PlotWindow(Uint32 id, std::string title);

	void SetCallback(CmplxFunc callback);

	bool OnCreate() override;
	bool OnEvent(const SDL_Event& e) override;
	bool OnUpdate(double frametime = 0) override;
	void OnRender(SDL_Renderer* renderer = nullptr) override;

private:
	Uint32 id;
	CmplxFunc callback;

	SDL_Texture* texture;
};