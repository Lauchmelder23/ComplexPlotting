#pragma once

#include <vector>

#include "PlotWindow.hpp"

typedef std::vector<PlotWindow> WList;

class PlotManager
{
public:
	static void NewPlot();
	static WList::iterator Remove(WList::iterator it);

	static void Loop();

public:
	inline static bool isOpen = true;

private:
	inline static WList OpenWindows;
	inline static int currentID = 0;
};