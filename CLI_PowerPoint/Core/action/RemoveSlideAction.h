#pragma once

#include "IAction.h"
#include "AddSlideAction.h"
#include "../../Models/Slide.h"
#include <stdexcept>

namespace editing
{
	class RemoveSlideAction : public IAction
	{
	public:
		RemoveSlideAction(size_t pos) : m_pos(pos), m_removedSlide(nullptr) {}

		void doAction(document::Presentation& presentation) override
		{
			if (m_pos >= presentation.getSlideCount()) {
				throw std::out_of_range("Cannot remove slide: index out of range");
			}

			// Store the slide before removing (for undo)
			m_removedSlide = std::make_shared<document::Slide>(presentation.getSlide(m_pos));

			presentation.removeSlide(m_pos);
		}

		std::unique_ptr<IAction> createInverse() override
		{
			if (!m_removedSlide) {
				throw std::runtime_error("Cannot create inverse: no slide was removed");
			}

			return std::make_unique<AddSlideAction>(m_pos, m_removedSlide);
		}

	private:
		size_t m_pos;
		std::shared_ptr<document::Slide> m_removedSlide;
	};
}