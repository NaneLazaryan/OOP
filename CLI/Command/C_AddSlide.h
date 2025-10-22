#pragma once

#include "Command.h"

class C_AddSlide : public Command
{
public:
public:
	C_AddSlide() = default;

	void execute() override
	{
		std::cout << "\nAdd Slide execution\n";

		if (arguments.empty())
			throw std::invalid_argument("Add Slide command requires arguments");

	}
};