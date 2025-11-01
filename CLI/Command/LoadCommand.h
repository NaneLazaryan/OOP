#pragma once

#include "Command.h"
#include "Serializer.h"

using namespace document;

namespace cli::cmd
{
	class LoadCommand : public BaseCommand
	{
	public:
		LoadCommand() = default;
		~LoadCommand() = default;

		void execute(Presentation& pres)
		{
			std::cout << "\nExecuting Load command\n";

			if (!hasArgument("text")) {
				throw std::invalid_argument("Filename is required for load command");
			}

			const Argument* filenameArg = getArgument("text");
			if (!filenameArg || !filenameArg->hasValue() || !filenameArg->isString()) {
				throw std::invalid_argument("Invalid filename");
			}

			std::string filename = filenameArg->getStrValue();

			if (filename.length() < 5 || filename.substr(filename.length() - 5) != ".json") {
				filename += ".json";
			}

			std::cout << "Loading presentation from: " << filename << std::endl;

			if (!JsonSerializer::loadFromFile(pres, filename)) {
				throw std::runtime_error("Failed to load presentation from file: " + filename);
			}

			std::cout << "Presentation loaded successfully!" << std::endl;
			std::cout << "Total slides: " << pres.getSlideCount() << std::endl;

			for (size_t i = 0; i < pres.getSlideCount(); i++) {
				SlidePtr slide = pres.getSlide(i);
				std::cout << "  Slide " << slide->getId() << ": " << slide->getShapeCount() << " shapes" << std::endl;
			}
		}
	};
}