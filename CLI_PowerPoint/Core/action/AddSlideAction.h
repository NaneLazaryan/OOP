#pragma once

#include "IAction.h"
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

		void doAction(document::Presentation& presentation);
		std::unique_ptr<IAction> createInverse();

	private:
		size_t m_pos;
		std::shared_ptr<document::Slide> m_slide;
	};
}