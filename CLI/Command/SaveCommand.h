#pragma once

#include "Command.h"
#include "Serializer.h"
#include <iostream>

using namespace document;

namespace cli::cmd
{
	class SaveCommand : public BaseCommand
	{
	public:
		SaveCommand() = default;
		~SaveCommand() = default;

		void execute(Presentation& pres)
		{
			std::cout << "\nExecuting Save command\n";

			if (!hasArgument("text")) {
				throw std::invalid_argument("Filename is required for save command");
			}

			const Argument* fileNameArg = getArgument("text");
			if (!fileNameArg || !fileNameArg->hasValue() || !fileNameArg->isString()) {
				throw std::invalid_argument("Invalid filename");
			}

			std::string filename = fileNameArg->getStrValue();

			if (filename.length() < 5 || filename.substr(filename.length() - 5) != ".json") {
				filename += ".json";
			}

			std::cout << "Saving presentation to: " << filename << std::endl;

			if (!JsonSerializer::saveToFile(pres, filename)) {
				throw std::runtime_error("Failed to save presentation to file: " + filename);
			}

			std::cout << "Presentation saved successfully!" << std::endl;
			std::cout << "File: " << filename << std::endl;
			std::cout << "Total slides: " << pres.getSlideCount() << std::endl;
		}
	};
}