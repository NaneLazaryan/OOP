#include "AddSlideAction.h"
#include "RemoveSlideAction.h"

namespace editing
{
    void AddSlideAction::doAction(document::Presentation& presentation)
    {
        presentation.addSlide(m_pos, m_slide);
    }

    std::unique_ptr<IAction> AddSlideAction::createInverse()
    {
        return std::make_unique<RemoveSlideAction>(m_pos);
    }
}