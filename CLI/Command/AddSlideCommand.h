#pragma once

#include "Command.h"

class AddSlideCommand : public Command
{
public:
public:
	AddSlideCommand() = default;

	void execute() override
	{
		std::cout << "\nAdd Slide execution\n";

		if (arguments.empty())
			throw std::invalid_argument("Add Slide command requires arguments");

	}
};