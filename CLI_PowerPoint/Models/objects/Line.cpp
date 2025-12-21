#include "Line.h"
#include "../../Core/vizualization/IVisitor.h"

namespace document::shapes
{
    Line::Line(const utility::Geometry& geometry)
        : Object(ObjectType::Line, geometry),
        m_lineColor(utility::Color::Black()),
        m_thickness(1.0)
    {}

    Line::Line(const utility::Geometry& geometry, const utility::Color& color, double thickness)
        : Object(ObjectType::Line, geometry),
        m_lineColor(color),
        m_thickness(thickness)
    {}

    std::string Line::getName() const
    {
        return "Line";
    }

    std::unique_ptr<IObject> Line::clone() const
    {
        return std::make_unique<Line>(m_geometry, m_lineColor, m_thickness);
    }

    void Line::accept(vizualization::IVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}