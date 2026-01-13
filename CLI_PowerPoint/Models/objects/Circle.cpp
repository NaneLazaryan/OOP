#include "Circle.h"
#include "../IObjectVisitor.h"

namespace document::shapes
{
    Circle::Circle(const utility::Geometry& geometry)
        : Object(ObjectType::Circle, geometry),
        m_fillColor(utility::Color::White()),
        m_border(utility::Border())
    {}

    Circle::Circle(const utility::Geometry& geometry, const utility::Color& fillColor, const utility::Border& border)
        : Object(ObjectType::Circle, geometry),
        m_fillColor(fillColor),
        m_border(border)
    {}

    std::string Circle::getName() const
    {
        return "Circle";
    }

    std::unique_ptr<IObject> Circle::clone() const
    {
        return std::make_unique<Circle>(m_geometry, m_fillColor, m_border);
    }

    void Circle::accept(IObjectVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}