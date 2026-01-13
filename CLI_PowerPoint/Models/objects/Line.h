#pragma once
#include "../Object.h"

namespace document::shapes
{
    class Line : public Object
    {
    public:
        Line(const utility::Geometry& geometry);
        Line(const utility::Geometry& geometry, const utility::Color& color, double thickness = 1.0);

        std::string getName() const;
        std::unique_ptr<IObject> clone() const;
        void accept(IObjectVisitor& visitor) const;

        // Line-specific properties
        const utility::Color& getLineColor() const { return m_lineColor; }
        void setLineColor(const utility::Color& color) { m_lineColor = color; }

        double getThickness() const { return m_thickness; }
        void setThickness(double thickness) { m_thickness = thickness; }

    private:
        utility::Color m_lineColor;
        double m_thickness;
    };
}