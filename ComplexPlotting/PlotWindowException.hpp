#pragma once

#include "PlotWindow.hpp"

class PlotWindowException :
	public std::exception
{
public:
	PlotWindowException(const char* message, PlotWindow* src) :
		std::exception(message), src(src)
	{
	}

	PlotWindow* where() const noexcept
	{
		return src;
	}

protected:
	PlotWindow* src;
};