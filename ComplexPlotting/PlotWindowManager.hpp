#include "PlotWindow.hpp"

#include "PlotWindowException.hpp"

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
		PlotWindow* plt = new PlotWindow(PlotWindowCount, title, -5.f, 5.f, -5.f, 5.f);
		plt->Open();

		// TODO: remove random values
		float a = (float)(rand() % 200 - 100) / 100.f;
		float b = (float)(rand() % 200 - 100) / 100.f;
		plt->SetCallback(std::bind([a, b](std::complex<float> c)
			{
				/*
				return std::complex<float>{1, 1} / (c + std::complex<float>{1, 0})
					+ std::complex<float>{2, 1} / (c + std::complex<float>{-1, 0});
				*/

				return std::tan(std::sin(c));
			},
			std::placeholders::_1));
		plt->DrawTexture();

		PlotWindows.insert({ PlotWindowCount, plt });
		PlotWindowCount++;

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
				try
				{
					PlotWindow* newWindow = MakeNew(title);
					PrintThreadSafe("-- Success\n");
				}
				catch (PlotWindowException e)
				{
					PrintThreadSafe(e.what());
					PrintThreadSafe("\n");
					PrintThreadSafe("-- Failed.\n");

					PlotWindow* src = e.where();
					src->Stop();
					delete src;
					src = nullptr;
				}
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