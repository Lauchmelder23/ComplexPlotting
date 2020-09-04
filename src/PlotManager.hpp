#pragma once

#include <vector>

#include <thread>
#include <future>

#include <imgui/imstb_truetype.h>

#include "PlotWindow.hpp"
#include "objects/Shader.hpp"

typedef std::vector<PlotWindow> WList;

class PlotManager
{
public:
	static void NewPlot(std::string title);
	static void Loop();

	static void Close();

public:
	inline static bool isOpen = true;
	inline static int currentID = 0;

private:
	inline static PlotWindow* OpenWindow = nullptr;
};