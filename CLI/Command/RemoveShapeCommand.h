#pragma once

#include "Command.h"

using namespace document;
using namespace document::shapes;

namespace cli::cmd
{
	class RemoveShapeCommand : public BaseCommand
	{
	public:
		RemoveShapeCommand() = default;
		~RemoveShapeCommand() = default;

		void execute(Presentation& pres) override
		{
			std::cout << "\nExecuting Remove Shape command\n";

			// Get slide index 
			if (!hasArgument("at")) {
				throw std::invalid_argument("Slide index (-at) is required for removing shapes");
			}

			const Argument* slideArg = getArgument("at");
			if (!slideArg || !slideArg->hasValue() || !slideArg->isInt()) {
				throw std::invalid_argument("Invalid slide index");
			}

			int slideIndex = slideArg->getIntValue();
			if (slideIndex < 0 || slideIndex >= pres.getSlideCount()) {
				throw std::out_of_range("Slide index out of range");
			}

			SlidePtr slide = pres.getSlide(static_cast<size_t>(slideIndex));
			if (slide->getShapeCount() == 0) {
				throw std::runtime_error("No shapes to remove from this slide");
			}

			// Get removable shape type
			size_t shapeIndex = slide->getShapeCount() - 1;
			if (hasArgument("shape")) {
				const Argument* arg = getArgument("shape");
				if (arg && arg->hasValue() && arg->isInt()) {
					int index = arg->getIntValue();
					if (index < 0) {
						throw std::invalid_argument("Shape index cannot be negative");
					}
					shapeIndex = static_cast<size_t>(index);
				}
			}

			if (shapeIndex >= slide->getShapeCount()) {
				throw std::out_of_range("Shape index out of range");
			}

			slide->removeShape(shapeIndex);

			std::cout << "Shape at index " << shapeIndex << " removed from slide " << slideIndex << std::endl;
			std::cout << "Remaining shapes on slide: " << slide->getShapeCount() << std::endl;
		}
	};
}
