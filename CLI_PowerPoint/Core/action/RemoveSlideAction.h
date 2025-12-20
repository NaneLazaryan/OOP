#pragma once

#include "IAction.h"
#include "../../Models/Slide.h"
#include <stdexcept>

namespace editing
{
	class AddSlideAction;

	class RemoveSlideAction : public IAction
	{
	public:
		RemoveSlideAction(size_t pos) : m_pos(pos), m_removedSlide(nullptr) {}

		void doAction(document::Presentation& presentation);
		std::unique_ptr<IAction> createInverse();

	private:
		size_t m_pos;
		std::shared_ptr<document::Slide> m_removedSlide;
	};
}