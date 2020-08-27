#include "PlotWindow.hpp"

#include <map>
#include <thread>
#include <future>

class PlotWindowManager
{
public:
	PlotWindowManager() = delete;
	PlotWindowManager& operator=(const PlotWindowManager& other) = delete;

	static void MakeNew(std::string title)
	{
		PlotWindow* plt = new PlotWindow(PlotWindowCount, title);
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
		// Check for input from the console
		if (InputFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
		{
			MakeNew(InputFuture.get());

			InputPromise = std::promise<std::string>();
			InputFuture = InputPromise.get_future();

			InputThread.join();
			InputThread = std::thread(PlotWindowManager::InputThreadFunc);
		}

		for (std::map<Uint32, PlotWindow*>::iterator it = PlotWindows.begin(); it != PlotWindows.end(); it++)
			it->second->OnUpdate();
	}

	static void Render()
	{
		for (std::map<Uint32, PlotWindow*>::iterator it = PlotWindows.begin(); it != PlotWindows.end(); it++)
			it->second->OnRender();
	}

	static void Quit()
	{
		InputThread.detach();
	}

private:
	static inline Uint32 PlotWindowCount = 1;
	static inline std::map<Uint32, PlotWindow*> PlotWindows;

	static inline std::promise<std::string> InputPromise;
	static inline std::future<std::string> InputFuture = InputPromise.get_future();

private:
	static void InputThreadFunc()
	{
		std::string in;
		std::cout << "> ";
		std::getline(std::cin, in);
		InputPromise.set_value(in);
	}

private:
	static inline std::thread InputThread = std::thread(PlotWindowManager::InputThreadFunc);
};