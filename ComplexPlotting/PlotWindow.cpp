#include "PlotWindow.hpp"
#include "PlotWindowException.hpp"

#include <iostream>

#define PI 3.1415926535f

SDL_Color HSVtoRGB(float H, float S, float V)
{
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	Uint8 R = (r + m) * 255;
	Uint8 G = (g + m) * 255;
	Uint8 B = (b + m) * 255;

	return SDL_Color{ R, G, B, 255 };
}

PlotWindow::PlotWindow(Uint32 id, std::string title) :
	IWindow::IWindow(
		UnitVector2u * 400, 
		UnitVector2i * SDL_WINDOWPOS_UNDEFINED,
		"Plot " + std::to_string(id) + " [" + title + "]",
		NULL),
	id(id), texture(nullptr)
{

}

void PlotWindow::SetCallback(CmplxFunc callback)
{
	this->callback = callback;
}

bool PlotWindow::OnCreate()
{
	int w = 0, h = 0;
	SDL_GetWindowSize(m_pWindow, &w, &h);
	texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	if (texture != nullptr)
		throw PlotWindowException("Failed to create SDL_Texture.", this);

	return true;
}

bool PlotWindow::OnEvent(const SDL_Event& e)
{
	if (e.window.windowID != m_uWindowID)
		return true;

	switch (e.window.event)
	{
	case SDL_WINDOWEVENT_CLOSE:
		m_isWindowOpen = false;
		return false;

	default:
		break;
	}

	return true;
}

bool PlotWindow::OnUpdate(double frametime)
{
	return false;
}

void PlotWindow::OnRender(SDL_Renderer* renderer)
{
	float a = std::arg(callback(std::complex<float>{0.f, 0.f}));
	a = a / PI * 180;
	if (a < 0)
		a = 360 + a;

	SDL_Color c = HSVtoRGB(a, 100, 100);

	SDL_SetRenderDrawColor(m_pRenderer, c.r, c.g, c.b, c.a);
	SDL_RenderClear(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);

	return;
}
