#pragma once
#include "Color.h"

namespace document::shapes::utility
{
    class Border
    {
    public:
        Border()
            : m_color(Color::Black()),
            m_thickness(1.0),
            m_visible(true)
        {}

        Border(const Color& color, double thickness, bool visible = true)
            : m_color(color),
            m_thickness(thickness),
            m_visible(visible)
        {}

        // Getters
        const Color& getColor() const { return m_color; }
        double getThickness() const { return m_thickness; }
        bool isVisible() const { return m_visible; }

        // Setters
        void setColor(const Color& color) { m_color = color; }
        void setThickness(double thickness) { m_thickness = thickness; }
        void setVisible(bool visible) { m_visible = visible; }

    private:
        Color m_color;
        double m_thickness;
        bool m_visible;
    };
}