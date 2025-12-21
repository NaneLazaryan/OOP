#pragma once

#include "IAction.h"
#include "../../Models/Slide.h"

namespace editing
{
	class RemoveSlideAction;

	class AddSlideAction : public IAction
	{
	public:
		AddSlideAction(size_t pos, std::shared_ptr<document::Slide> slide)
			: m_pos(pos), m_slide(slide) {}

		void doAction(document::Presentation& presentation)
		{
			size_t slideCount = presentation.getSlideCount();
			if (m_pos > slideCount) {
				m_pos = slideCount;
			}
			presentation.addSlide(m_pos, m_slide);
		}

		std::unique_ptr<IAction> createInverse();

	private:
		size_t m_pos;
		std::shared_ptr<document::Slide> m_slide;
	};
}