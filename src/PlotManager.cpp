#include "PlotManager.hpp"

#include <iostream>

void PlotManager::NewPlot(std::string title)
{
	currentID++;
	OpenWindow = new PlotWindow(700, 700, currentID, 50, title);
}

void PlotManager::Loop()
{
	if (OpenWindow->ShouldClose())
	{
		Close();
		return;
	}

	OpenWindow->GiveContext();
	OpenWindow->Clear();
	OpenWindow->Display();
}

void PlotManager::Close()
{
	OpenWindow->Destroy();
	delete OpenWindow;
	OpenWindow = nullptr;
	isOpen = false;
}