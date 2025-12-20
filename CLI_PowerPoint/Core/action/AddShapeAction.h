#pragma once

#include "IAction.h"
#include "../../Models/IObject.h"
#include <stdexcept>

namespace editing { class RemoveShapeAction; }


namespace editing
{
	class AddShapeAction : public IAction
	{
	public:
		AddShapeAction(int slideId, std::unique_ptr<document::shapes::IObject> shape)
			: m_slideId(slideId), m_shape(std::move(shape)), m_shapeIndex(static_cast<size_t>(-1))
		{}

        void doAction(document::Presentation& presentation) override
        {
            // Find the slide by ID
            size_t slideIndex = findSlideIndexById(presentation, m_slideId);
            if (slideIndex == static_cast<size_t>(-1)) {
                throw std::runtime_error("Slide not found with ID: " + std::to_string(m_slideId));
            }

            document::Slide& slide = presentation.getSlide(slideIndex);
            /*if (slide) {
                throw std::runtime_error("Cannot add shape: slide is null");
            }*/

            // Store the shape index
            m_shapeIndex = slide.getShapeCount();

            slide.addShape(std::move(m_shape));
        }

        std::unique_ptr<IAction> createInverse() override
        {
            if (m_shapeIndex == static_cast<size_t>(-1)) {
                throw std::runtime_error("Cannot create inverse: shape was not added");
            }
            return std::make_unique<RemoveShapeAction>(m_slideId, m_shapeIndex);
        }

	private:
		int m_slideId;
		std::unique_ptr<document::shapes::IObject> m_shape;
        size_t m_shapeIndex;

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