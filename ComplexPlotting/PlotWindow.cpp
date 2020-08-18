#include "PlotWindow.hpp"

PlotWindow::PlotWindow(std::string title) :
	IWindow::IWindow(
		UnitVector2u * 400, 
		UnitVector2i * SDL_WINDOWPOS_UNDEFINED,
		title,
		NULL)
{
}

void PlotWindow::OnEvent(const SDL_Event& e)
{
	if (e.window.windowID != m_uWindowID)
		return;

	switch (e.window.type)
	{
	case SDL_WINDOWEVENT_CLOSE:
		m_isWindowOpen = false;
		break;
	}
}

bool PlotWindow::OnUpdate(double frametime)
{
	return false;
}

void PlotWindow::OnRender(SDL_Renderer* renderer)
{
	return;
}
