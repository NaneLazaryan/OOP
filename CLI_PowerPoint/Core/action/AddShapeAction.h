#pragma once

#include "IAction.h"
#include "../../Models/IObject.h"
#include <stdexcept>

namespace editing
{
    class RemoveShapeAction;

	class AddShapeAction : public IAction
	{
	public:
		AddShapeAction(int slideId, std::unique_ptr<document::shapes::IObject> shape)
			: m_slideId(slideId), m_shape(std::move(shape)), m_shapeIndex(static_cast<size_t>(-1))
		{}

		void doAction(document::Presentation& presentation);
        std::unique_ptr<IAction> createInverse();

	private:
		int m_slideId;
		std::unique_ptr<document::shapes::IObject> m_shape;
        size_t m_shapeIndex;

	private:
		size_t findSlideIndexById(document::Presentation& presentation, int id);
	};
}