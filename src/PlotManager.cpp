#include "PlotManager.hpp"

#include <iostream>

void PlotManager::NewPlot(std::string title)
{
	currentID++;
	OpenWindows.emplace_back(400, 400, currentID, title);
}

WList::iterator PlotManager::Remove(WList::iterator it)
{
	return OpenWindows.erase(it);
}

void PlotManager::Loop()
{
	// Check if the future has been set
	if (inputFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
	{
		inputThread.join();

		std::string input = inputFuture.get();
		for (int i = 0; i < (sizeof(QUIT_COMMANDS) / sizeof(uint64_t)); i++)
		{
			if (input == QUIT_COMMANDS[i])
			{
				Close();
				return;
			}
		}

		NewPlot(input);

		inputPromise = std::promise<std::string>();
		inputFuture = inputPromise.get_future();

		inputThread = std::thread(&PlotManager::InputThreadFunction);
	}

	for (WList::iterator it = OpenWindows.begin(); it != OpenWindows.end();)
	{
		if (it->ShouldClose())
		{
			it->Destroy();
			it = OpenWindows.erase(it);
			
			continue;
		}

		it->GiveContext();

		it->Clear();
		it->Display();
		it++;
	}
}

void PlotManager::Close()
{
	for (WList::iterator it = OpenWindows.begin(); it != OpenWindows.end();)
	{
		it->Destroy();
		it = OpenWindows.erase(it);
	}

	try {
		inputThread.detach();
	} catch(...) {}
	isOpen = false;
}

void PlotManager::InputThreadFunction()
{
	PlotMgrMutex.lock();
	std::cout << "> ";
	PlotMgrMutex.unlock();

	std::string input;
	std::cin >> input;

	inputPromise.set_value(input);
}
