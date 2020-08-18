#include "PlotWindow.hpp"

#include <vector>

class PlotWindowManager
{
public:
	PlotWindowManager() = delete;
	PlotWindowManager& operator=(const PlotWindowManager& other) = delete;

	static void MakeNew()
	{
		PlotWindows.emplace_back("Plot " + std::to_string(PlotWindowCount));
		PlotWindowCount++;
	}

	static void HandleEvents()
	{
		for(auto plot : PlotWIndows)

	}

private:
	static inline Uint32 PlotWindowCount;
	static std::vector<PlotWindow> PlotWindows;
};