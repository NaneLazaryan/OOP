#pragma once

#include "Command.h"

class RemoveSlideCommand : public BaseCommand
{
public:
    RemoveSlideCommand() = default;
	~RemoveSlideCommand() = default;

	void execute(Presentation& pres) override
	{
        std::cout << "\Remove Slide execution\n";

        if (pres.getSlideCount() == 0)
            throw std::runtime_error("No slide to remove");

        size_t pos = pres.getSlideCount() - 1;

        if (hasArgument("at")) {
            const Argument* arg = getArgument("at");
            if (arg && arg->hasValue() && arg->isInt()) {
                int index = arg->getIntValue();
                if(index<0)
                    throw std::invalid_argument("Position can not be negative");
                
                pos = static_cast<size_t>(index);
            }
        }
        

        if (pos >= pres.getSlideCount()) {
            throw std::out_of_range("Slide index out of range");
        }

        presentation.removeSlide(pos);

        std::cout << "Slide at position " << pos << " removed" << std::endl;
        std::cout << "Remaining slides: " << presentation.getSlideCount() << std::endl;
	}
};