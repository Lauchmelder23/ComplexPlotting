#include "PlotManager.hpp"

void PlotManager::NewPlot()
{
	currentID++;
	OpenWindows.emplace_back(400, 400, currentID);
}

WList::iterator PlotManager::Remove(WList::iterator it)
{
	return OpenWindows.erase(it);
}

void PlotManager::Loop()
{
	for (WList::iterator it = OpenWindows.begin(); it != OpenWindows.end();)
	{
		if (it->ShouldClose())
		{
			it->Destroy();
			it = OpenWindows.erase(it);

			if (OpenWindows.empty())
				isOpen = false;
			
			continue;
		}

		it->GiveContext();

		it->Clear();
		it->Display();
		it++;
	}
}