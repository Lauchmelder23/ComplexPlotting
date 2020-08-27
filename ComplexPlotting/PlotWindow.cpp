#include "PlotWindow.hpp"

#include <iostream>

PlotWindow::PlotWindow(Uint32 id) :
	IWindow::IWindow(
		UnitVector2u * 400, 
		UnitVector2i * SDL_WINDOWPOS_UNDEFINED,
		"Plot " + std::to_string(id),
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
	return;
}
