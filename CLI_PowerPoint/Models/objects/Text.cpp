#include "Text.h"
#include "../IObjectVisitor.h"

namespace document::shapes
{
    Text::Text(const utility::Geometry& geometry, const std::string& text)
        : Object(ObjectType::Text, geometry),
        m_text(text),
        m_textColor(utility::Color::Black()),
        m_backgroundColor(utility::Color::Transparent()),
        m_border(utility::Border(utility::Color::Black(), 0.0, false))
    {}

    Text::Text(const utility::Geometry& geometry, const std::string& text, const utility::Color& textColor, const utility::Color& backgroundColor, const utility::Border& border)
        : Object(ObjectType::Text, geometry),
        m_text(text),
        m_textColor(textColor),
        m_backgroundColor(backgroundColor),
        m_border(border)
    {}

    std::string Text::getName() const
    {
        return "Text";
    }

    std::unique_ptr<IObject> Text::clone() const
    {
        return std::make_unique<Text>(m_geometry, m_text, m_textColor, m_backgroundColor, m_border);
    }

    void Text::accept(IObjectVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}