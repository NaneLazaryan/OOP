#pragma once

#include "Command.h"

using namespace document;

namespace cli::cmd
{
	class AddSlideCommand : public BaseCommand
	{
	public:
		AddSlideCommand() = default;
		~AddSlideCommand() = default;

		void execute(Presentation& pres) override
		{
			std::cout << "\nExecuting Add Slide command\n";

			size_t pos = pres.getSlideCount();

			if (hasArgument("at")) {
				const Argument* arg = getArgument("at");
				if (arg && arg->hasValue() && arg->isInt()) {
					int posValue = arg->getIntValue();

					if (posValue < 0)
						throw std::invalid_argument("Position can not be negative");

					if (static_cast<size_t>(posValue) > pres.getSlideCount())
						throw std::out_of_range("Position out of range");

					pos = static_cast<size_t>(posValue);
				}
			}
			else if (!arguments.empty())
				throw std::invalid_argument("Add Slide has no such argument.");

			auto newSlide = std::make_shared<Slide>();
			pres.addSlide(pos, newSlide);

			std::cout << "Slide added at position " << pos << std::endl;
			std::cout << "Total slides: " << pres.getSlideCount() << std::endl;
		}
	};
}
