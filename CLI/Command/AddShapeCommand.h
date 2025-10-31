#pragma once
#include "Command.h"
#include "ShapeFactory.h"
#include <iostream>

class AddShapeCommand : public BaseCommand
{
public:
	 AddShapeCommand() = default;
	~AddShapeCommand() = default;

	void execute(Presentation& pres) override
	{
		std::cout << "\nExecuting Add Shape command\n" << std::endl;

		size_t slideId = 0; 

		// Get slide index (default 0)
		if (hasArgument("at")) {
			const Argument* arg = getArgument("at");
			if (arg && arg->hasValue() && arg->isInt()) {
				int index = arg->getIntValue();
				if (index < 0)
					throw std::invalid_argument("Index can not be negative");
				slideId = static_cast<size_t>(index);
			}
		}

		if (slideId >= pres.getSlideCount())
			throw std::out_of_range("Slide index " + std::to_string(slideId) + " out of range. Total slides: " + std::to_string(pres.getSlideCount()));

		// Get shape type (default rectangle)
		std::string shapeType = "rectangle"; 
		if (hasArgument("type")) {
			const Argument* arg = getArgument("type");
			if (arg && arg->hasValue() && arg->isString()) {
				shapeType = arg->getStrValue();
			}
		}

		// Get position for shape (default 0,0)
		float x = 0.0f, y = 0.0f;
		if (hasArgument("pos")) {
			const Argument* arg = getArgument("pos");
			if (arg && arg->hasValue() && arg->isCoordinate()) {
				auto pos = arg->getPairValue();
				x = static_cast<float>(pos.first);
				y = static_cast<float>(pos.second);
			}
		}

		SlidePtr slide = pres.getSlide(slideId);
		try {
			ShapePtr shape = ShapeFactory::createShape(shapeType);
			if (hasArgument("pos")) {
				shape->setPosition(x, y);
			}
			slide->addShape(shape);
		}
		catch (const std::invalid_argument& ex) {
			throw std::invalid_argument("Invalid shape type '" + shapeType + "': " + ex.what());
		}

		std::cout << "Shape '" << shapeType << "' added to slide " << slideId << " at position (" << x << ", " << y << ")" << std::endl;
		std::cout << "Slide now has " << slide->getShapeCount() << " shapes" << std::endl;
	}
};