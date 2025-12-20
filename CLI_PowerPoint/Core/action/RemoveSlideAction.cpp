#include "RemoveSlideAction.h"
#include "AddSlideAction.h"

namespace editing
{
	void RemoveSlideAction::doAction(document::Presentation& presentation)
	{
		if (m_pos >= presentation.getSlideCount()) {
			throw std::out_of_range("Cannot remove slide: index out of range");
		}

		// Store the slide before removing (for undo)
		m_removedSlide = std::make_shared<document::Slide>(presentation.getSlide(m_pos));

		presentation.removeSlide(m_pos);
	}

    std::unique_ptr<IAction> RemoveSlideAction::createInverse()
    {
        if (!m_removedSlide) {
            throw std::runtime_error("Cannot create inverse: no slide was removed");
        }
        return std::make_unique<AddSlideAction>(m_pos, m_removedSlide);
    }
}