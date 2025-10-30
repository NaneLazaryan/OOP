#pragma once
#include "Command.h"
#include <iostream>

class SetTitleCommand : public Command
{
public:
	~SetTitleCommand() = default;

	void execute() override
	{
		std::cout << "Executing SetTitle command" << std::endl;
		
	}
};