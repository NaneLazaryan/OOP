#pragma once

#include "IAction.h"
#include "IAction.h"
#include "../../Models/Slide.h"

namespace editing { class RemoveSlideAction; }

namespace editing
{
	class AddSlideAction : public IAction
	{
	public:
		AddSlideAction(size_t pos, std::shared_ptr<document::Slide> slide) 
			: m_pos(pos), m_slide(slide) {}

		void doAction(document::Presentation& presentation) override
		{
			presentation.addSlide(m_pos, m_slide);
			/*std::cout << "Slide added at position " << m_pos << std::endl;
			std::cout << "Total slides: " << pres.getSlideCount() << std::endl;*/
		}

		std::unique_ptr<IAction> createInverse() override
		{
			return std::make_unique<RemoveSlideAction>(m_pos);
		}

	private:
		size_t m_pos;
		std::shared_ptr<document::Slide> m_slide;
	};
}