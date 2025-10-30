#pragma once
#include "Command.h"
#include <iostream>

class AddShapeCommand : public Command
{
public:
	~AddShapeCommand() = default;

	void execute() override
	{
		std::cout << "Executing AddShape command" << std::endl;
		std::cout << "Validation..." << std::endl;

		if (arguments.empty())
			throw std::invalid_argument("Add Slide command requires arguments");

		if (!hasArgument("type"))
			throw std::runtime_error("Shape type is required");

		std::string shapeType = getArgument("type")->getStrValue();

		if (hasArgument("pos"))
			auto pos = getArgument("pos")->getPairValue();
		else
			auto pos = std::pair<double, double>{ 0.0,0.0 };
	}
};