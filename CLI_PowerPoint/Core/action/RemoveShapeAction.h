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

		void doAction(document::Presentation& presentation);

		std::unique_ptr<IAction> createInverse();
	private:
		int m_slideId;
		size_t m_shapeIndex;
		std::unique_ptr<document::shapes::IObject> m_removedShape;

	private:
		size_t findSlideIndexById(document::Presentation& presentation, int id);
	};
}