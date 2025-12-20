#include "AddShapeAction.h"
#include "RemoveShapeAction.h"
#include "AddSlideAction.h"
#include "RemoveSlideAction.h"

namespace editing
{
    // AddShapeAction::createInverse
    std::unique_ptr<IAction> AddShapeAction::createInverse()
    {
        if (m_shapeIndex == static_cast<size_t>(-1)) {
            throw std::runtime_error("Cannot create inverse: shape was not added");
        }
        return std::make_unique<RemoveShapeAction>(m_slideId, m_shapeIndex);
    }

    // RemoveShapeAction::createInverse
    std::unique_ptr<IAction> RemoveShapeAction::createInverse()
    {
        if (!m_removedShape) {
            throw std::runtime_error("Cannot create inverse: no shape was removed");
        }

        auto clonedShape = m_removedShape->clone();
        return std::make_unique<AddShapeAction>(m_slideId, std::move(clonedShape));
    }

    // AddSlideAction::createInverse
    std::unique_ptr<IAction> AddSlideAction::createInverse()
    {
        return std::make_unique<RemoveSlideAction>(m_pos);
    }

    // RemoveSlideAction::createInverse
    std::unique_ptr<IAction> RemoveSlideAction::createInverse()
    {
        if (!m_removedSlide) {
            throw std::runtime_error("Cannot create inverse: no slide was removed");
        }
        return std::make_unique<AddSlideAction>(m_pos, m_removedSlide);
    }
}