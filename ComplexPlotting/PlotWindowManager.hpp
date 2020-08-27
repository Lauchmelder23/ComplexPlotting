#include "PlotWindow.hpp"

#include <map>

class PlotWindowManager
{
public:
	PlotWindowManager() = delete;
	PlotWindowManager& operator=(const PlotWindowManager& other) = delete;

	static void MakeNew()
	{
		PlotWindow* plt = new PlotWindow(PlotWindowCount);
		PlotWindows.insert({ PlotWindowCount, plt });
		PlotWindowCount++;
		plt->Open();
	}

	static void HandleEvents(const SDL_Event& e)
	{
		for (std::map<Uint32, PlotWindow*>::iterator it = PlotWindows.begin(); it != PlotWindows.end(); )
		{
			if (!it->second->OnEvent(e))
			{
				it->second->Stop();
				delete it->second;
				it->second = NULL;
				it = PlotWindows.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	static void Update()
	{
		for (std::map<Uint32, PlotWindow*>::iterator it = PlotWindows.begin(); it != PlotWindows.end(); it++)
			it->second->OnUpdate();
	}

	static void Render()
	{
		for (std::map<Uint32, PlotWindow*>::iterator it = PlotWindows.begin(); it != PlotWindows.end(); it++)
			it->second->OnRender();
	}

private:
	static inline Uint32 PlotWindowCount = 1;
	static inline std::map<Uint32, PlotWindow*> PlotWindows;
};