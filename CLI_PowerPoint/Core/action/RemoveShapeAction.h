#pragma once

#include "IAction.h"
#include <stdexcept>

namespace editing
{
	class AddShapeAction; 

	class RemoveShapeAction : public IAction
	{
	public:
		RemoveShapeAction(int slideId, size_t shapeIndex)
			: m_slideId(slideId), m_shapeIndex(shapeIndex)
		{}

		void doAction(document::Presentation& presentation)
		{
			// Find the slide by ID
			size_t slideIndex = findSlideIndexById(presentation, m_slideId);
			if (slideIndex == static_cast<size_t>(-1)) {
				throw std::runtime_error("Slide not found with ID: " + std::to_string(m_slideId));
			}

			document::Slide& slide = presentation.getSlide(slideIndex);

			if (m_shapeIndex >= slide.getShapeCount()) {
				throw std::out_of_range("Cannot remove shape: index out of range");
			}

			// Store shape before removing (for undo)
			m_removedShape = slide.getShape(m_shapeIndex).clone();

			slide.removeShape(m_shapeIndex);
		}

		std::unique_ptr<IAction> createInverse();

	private:
		int m_slideId;
		size_t m_shapeIndex;
		std::unique_ptr<document::shapes::IObject> m_removedShape;

		size_t findSlideIndexById(document::Presentation& presentation, int id)
		{
			size_t index = 0;
			for (auto it = presentation.begin(); it != presentation.end(); ++it, ++index) {
				if ((*it)->getId() == id) {
					return index;
				}
			}
			return static_cast<size_t>(-1);
		}
	};
}