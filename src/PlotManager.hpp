#pragma once

#include <vector>

#include <thread>
#include <future>

#include "PlotWindow.hpp"

typedef std::vector<PlotWindow> WList;

class PlotManager
{
public:
	inline static const char* const QUIT_COMMANDS[2] = { "q", "quit" };
public:
	static void NewPlot(std::string title);
	static WList::iterator Remove(WList::iterator it);

	static void Loop();

	static void Close();

public:
	inline static bool isOpen = true;

private:
	inline static WList OpenWindows;
	inline static int currentID = 0;

	inline static std::promise<std::string> inputPromise;
	inline static std::future<std::string> inputFuture = inputPromise.get_future();

	inline static std::mutex PlotMgrMutex;

private:
	static void InputThreadFunction();

private:
	inline static std::thread inputThread = std::thread(&PlotManager::InputThreadFunction);

};