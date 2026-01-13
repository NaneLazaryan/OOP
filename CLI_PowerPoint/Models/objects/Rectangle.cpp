#include "Rectangle.h"
#include "../IObjectVisitor.h"

namespace document::shapes
{
    Rectangle::Rectangle(const utility::Geometry& geometry)
        : Object(ObjectType::Rectangle, geometry),
        m_fillColor(utility::Color::White()),
        m_border(utility::Border())
    {}

    Rectangle::Rectangle(const utility::Geometry& geometry, const utility::Color& fillColor, const utility::Border& border)
        : Object(ObjectType::Rectangle, geometry),
        m_fillColor(fillColor),
        m_border(border)
    {}

    std::string Rectangle::getName() const
    {
        return "Rectangle";
    }

    std::unique_ptr<IObject> Rectangle::clone() const
    {
        return std::make_unique<Rectangle>(m_geometry, m_fillColor, m_border);
    }

    void Rectangle::accept(IObjectVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}