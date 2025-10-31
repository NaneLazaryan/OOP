#pragma once

#include "Command.h"

class AddSlideCommand : public BaseCommand
{
public:
	AddSlideCommand() = default;
	~AddSlideCommand() = default;

	void execute(Presentation& pres) override
	{
		std::cout << "\nAdd Slide execution\n";

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
		
		auto newSlide = std::make_shared<Slide>();
		presentation.addSlide(pos, newSlide);

		std::cout << "Slide added at position " << pos << std::endl;
		std::cout << "Total slides: " << pres.getSlideCount() << std::endl;
	}
};