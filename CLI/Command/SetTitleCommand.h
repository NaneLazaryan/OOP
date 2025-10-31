#pragma once
#include "Command.h"
#include <iostream>

class SetTitleCommand : public BaseCommand
{
public:
	SetTitleCommand() = default;
	~SetTitleCommand() = default;

	void execute(Presentation& pres) override
	{
		std::cout << "\nExecuting Set Title command\n" << std::endl;
		
		// Get text for title
		if (!hasArgument("text")) {
			throw std::invalid_argument("Title text is required");
		}
		
		const Argument* textArg = getArgument("text");
		if (!textArg || !textArg->hasValue() || !textArg->isString()) {
			throw std::invalid_argument("Invalid title text");
		}

		std::string titleText = textArg->getStrValue();

		// Get slide index
		size_t slideId = 0;
		if (hasArgument("at")) {
			const Argument* arg = getArgument("at");
			if (arg && arg->hasValue() && arg->isInt()) {
				int index = arg->getIntValue();
				if (index < 0) {
					throw std::invalid_argument("Slide index cannot be negative");
				}
				slideId = static_cast<size_t>(index);
			}
		}

		if (slideId >= pres.getSlideCount()) {
			throw std::out_of_range("Slide index out of range");
		}

		SlidePtr slide = pres.getSlide(slideId);
	
		ShapePtr titleShape = ShapeFactory::createShape("Text");
		slide->addShape(titleShape);

		std::cout << "Title set to \"" << titleText << "\" on slide " << slideId << std::endl;
	}
};