
#include "AddShapeAction.h"
#include "RemoveShapeAction.h"

namespace editing
{
    void AddShapeAction::doAction(document::Presentation& presentation)
    {
        // Find the slide by ID
        size_t slideIndex = findSlideIndexById(presentation, m_slideId);
        if (slideIndex == static_cast<size_t>(-1)) {
            throw std::runtime_error("Slide not found with ID: " + std::to_string(m_slideId));
        }

        document::Slide& slide = presentation.getSlide(slideIndex);

        // Store the shape index
        m_shapeIndex = slide.getShapeCount();

        slide.addShape(std::move(m_shape));
    }


    std::unique_ptr<IAction> AddShapeAction::createInverse()
    {
        if (m_shapeIndex == static_cast<size_t>(-1)) {
            throw std::runtime_error("Cannot create inverse: shape was not added");
        }
        return std::make_unique<RemoveShapeAction>(m_slideId, m_shapeIndex);
    }

    size_t AddShapeAction::findSlideIndexById(document::Presentation& presentation, int id)
    {
        size_t index = 0;
        for (auto it = presentation.begin(); it != presentation.end(); ++it, ++index) {
            if ((*it)->getId() == id) {
                return index;
            }
        }
        return static_cast<size_t>(-1);
    }
}