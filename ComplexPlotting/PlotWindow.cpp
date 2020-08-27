#include "PlotWindow.hpp"

#include <iostream>

PlotWindow::PlotWindow(Uint32 id, std::string title) :
	IWindow::IWindow(
		UnitVector2u * 400, 
		UnitVector2i * SDL_WINDOWPOS_UNDEFINED,
		"Plot " + std::to_string(id) + " [" + title + "]",
		NULL),
	id(id)
{
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
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);

	return;
}
