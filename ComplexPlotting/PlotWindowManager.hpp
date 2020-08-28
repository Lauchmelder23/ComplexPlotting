#include "PlotWindow.hpp"

#include <map>
#include <thread>
#include <future>

class PlotWindowManager
{
public:
	PlotWindowManager() = delete;
	PlotWindowManager& operator=(const PlotWindowManager& other) = delete;

	static PlotWindow* MakeNew(std::string title)
	{
		PlotWindow* plt = new PlotWindow(PlotWindowCount, title);
		PlotWindows.insert({ PlotWindowCount, plt });
		PlotWindowCount++;
		plt->Open();

		return plt;
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
			std::string title = InputFuture.get();
			if (title != "")
			{
				PlotWindow* newWindow = MakeNew(title);
				newWindow->SetCallback(std::bind([](std::complex<float> c)
				{
					return std::complex<float>{ 1.f, 1.f };
				}, 
				std::placeholders::_1));

				PrintThreadSafe("-- Success\n");
			}

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

	static void PrintThreadSafe(std::string s, bool block = true)
	{
		if (block)
			StdOutMutex.lock();
		else
			if (!StdOutMutex.try_lock())
				return;

		std::cout << s;

		StdOutMutex.unlock();
	}

private:
	static inline Uint32 PlotWindowCount = 1;
	static inline std::map<Uint32, PlotWindow*> PlotWindows;

	static inline std::promise<std::string> InputPromise;
	static inline std::future<std::string> InputFuture = InputPromise.get_future();

	static inline std::mutex StdOutMutex;

private:
	static void InputThreadFunc()
	{
		std::string in;

		PrintThreadSafe("> ");
		std::getline(std::cin, in);

		InputPromise.set_value(in);
	}

private:
	static inline std::thread InputThread = std::thread(PlotWindowManager::InputThreadFunc);
};