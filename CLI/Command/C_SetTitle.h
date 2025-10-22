#pragma once
#include "Command.h"
#include <iostream>

class C_SetTitle : public Command
{
public:
	~C_SetTitle() = default;

	void execute() override
	{
		std::cout << "Executing SetTitle command" << std::endl;
		
	}
};